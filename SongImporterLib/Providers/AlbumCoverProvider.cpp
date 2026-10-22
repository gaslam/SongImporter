#include "AlbumCoverProvider.h"
#include "Song/Song.h"
#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/id3v2tag.h>
#include <taglib/fileref.h>
#include <taglib/aifffile.h>
#include <taglib/mp4file.h>

using namespace TagLib;

AlbumCoverProvider::AlbumCoverProvider(QString& defaultId,QImage& defaultLogo)
    : QQuickImageProvider{QQuickImageProvider::Image},
    m_DefaultCoverKey{defaultId}
{
    m_AlbumCovers[defaultId] = defaultLogo;
}

void AlbumCoverProvider::setImage(Song* song,FileRef& fileref)
{
    QString songId{QString("%1_%2").arg(song->albumArtists.toLower(),song->album.toLower())};

    extractAlbumCoverArt(fileref,songId,song);
}

void AlbumCoverProvider::extractAlbumCoverArt(FileRef& fileref, const QString& id, Song* song)
{
    File* file{fileref.file()};

    if(!file)
    {
        return;
    }

    /// If the album cover id already exists, set the cover id.
    {
        QMutexLocker locker{&m_Locker};
        if (m_AlbumCovers.contains(id)) {
            song->coverId = id;
            return;
        }
    }
    QImage image;

    /// Retrieve the album covers based on the file type.
    switch (song->format)
    {
    case SoftwareUtils::SupportedAudioFormats::FLAC:
    {
        auto *flac = dynamic_cast<TagLib::FLAC::File *>(file);

        const auto &pictures = flac->pictureList();
        if (!pictures.isEmpty()) {
            const auto data = pictures.front()->data();
            image = QImage::fromData(
                reinterpret_cast<const uchar *>(data.data()),
                data.size());
        }
        break;
    }

    case SoftwareUtils::SupportedAudioFormats::MP3:
    {
        auto *mpeg = dynamic_cast<TagLib::MPEG::File *>(file);
        image = extractImageFromTag(mpeg->ID3v2Tag());
        break;
    }

    case SoftwareUtils::SupportedAudioFormats::AIFF:
    case SoftwareUtils::SupportedAudioFormats::WAV:
    {
        auto *riff = dynamic_cast<TagLib::RIFF::File *>(file);

        if(auto* tag{dynamic_cast<ID3v2::Tag*>(riff->tag())})
        {
            image = extractImageFromTag(tag);
        }

        break;
    }

    case SoftwareUtils::SupportedAudioFormats::ALAC:
    {
        auto *mp4 = dynamic_cast<TagLib::MP4::File *>(file);

        if(auto* tag{dynamic_cast<ID3v2::Tag*>(mp4->tag())})
        {
            image = extractImageFromTag(tag);
        }
        break;
    }

    case SoftwareUtils::SupportedAudioFormats::AAC:
    case SoftwareUtils::SupportedAudioFormats::ZIP:
    case SoftwareUtils::SupportedAudioFormats::UNKNOWN:
    default:
        break;
    }

    if(!image.isNull())
    {
        QMutexLocker locker{&m_Locker};
        m_AlbumCovers[id] = image;
        song->coverId = id;
    }
    else
    {
        song->coverId = m_DefaultCoverKey;
    }
}

QImage AlbumCoverProvider::extractImageFromTag(ID3v2::Tag* tag) const
{
    if (tag)
    {
        /// Retrieve all APIC (Attached Picture) frames from the tag.
        /// These frames store embedded images, such as album covers.
        const TagLib::ID3v2::FrameList& frameList{tag->frameList("APIC")};

        /// Check if the tag contains at least one attached picture frame.
        if(!frameList.isEmpty())
        {

            /// The first frame is the album cover.
            auto pictureFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());

            /// Retrieve the picture's data
            const TagLib::ByteVector data = pictureFrame->picture();

            /// Decode the image data into a QImage and return it.
            /// We're not changing the raw data. No type checking is required so, reinterpret_cast was used for this case.
            return QImage::fromData(
                reinterpret_cast<const uchar*>(data.data()),
                data.size()
                );
        }
    }
    /// If all failse, return nothing.
    return{};
}

QImage AlbumCoverProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage image;
    {
        QMutexLocker locker{&m_Locker};
        if(m_AlbumCovers.contains(id))
        {
            image =  m_AlbumCovers[id];
        }
        else
        {
            image = m_AlbumCovers[m_DefaultCoverKey];
        }

        if (size)
            *size = image.size();

        if (!requestedSize.isValid())
            return image;
    }

    return image.scaled(requestedSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

bool AlbumCoverProvider::hasCover(Song* song)
{
    const QMutexLocker locker{&m_Locker};
    return m_AlbumCovers.contains(song->coverId);
}
