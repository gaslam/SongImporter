#include "AlbumCoverProvider.h"
#include "../Song.h"
#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/id3v2tag.h>
#include <taglib/fileref.h>

using namespace TagLib;

AlbumCoverProvider::AlbumCoverProvider(QString& defaultId,QImage& defaultLogo)
    : QQuickImageProvider{QQuickImageProvider::Image},
    m_DefaultCoverKey{defaultId}
{
    m_AlbumCovers[defaultId] = defaultLogo;
}

void AlbumCoverProvider::setImage(Song song,FileRef& fileref)
{
    QString songId{QString("%1_%2").arg(song.albumArtists.toLower(),song.album.toLower())};

    extractAlbumCoverArt(fileref,songId,song);
}

void AlbumCoverProvider::extractAlbumCoverArt(FileRef& fileref, const QString& id, Song& song)
{
    File* file{fileref.file()};

    if(!file)
    {
        return;
    }

    {
        QMutexLocker locker{&m_Locker};
        if (m_AlbumCovers.contains(id)) {
            song.coverId = id;
            return;
        }
    }

    QImage image{};
    if(FLAC::File* flac{dynamic_cast<FLAC::File*>(file)})
    {
        auto picList{flac->pictureList()};
        if(!picList.isEmpty())
        {
            const TagLib::FLAC::Picture* pic{ picList.front()};

            const TagLib::ByteVector data { pic->data()};

            image = QImage::fromData(
                reinterpret_cast<const uchar*>(data.data()),
                data.size()
                );
        }
    }

    if(image.isNull())
    {
        if (auto mpeg = dynamic_cast<TagLib::MPEG::File*>(file)) {
            TagLib::ID3v2::Tag *tag = mpeg->ID3v2Tag(); // may return nullptr
            if (tag)
            {
                const TagLib::ID3v2::FrameList& frameList{tag->frameList("APIC")};
                if(!frameList.isEmpty())
                {
                    auto pictureFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());

                    const TagLib::ByteVector data = pictureFrame->picture();
                    image = QImage::fromData(
                        reinterpret_cast<const uchar*>(data.data()),
                        data.size()
                        );
                }
            }
        }
    }

    QMutexLocker locker{&m_Locker};

    if(!image.isNull())
    {
        m_AlbumCovers[id] = image;
        song.coverId = id;
    }
    else
    {
        song.coverId = m_DefaultCoverKey;
    }
}

QImage AlbumCoverProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QMutexLocker locker{&m_Locker};
    if(m_AlbumCovers.contains(id))
    {
        return m_AlbumCovers[id];
    }
    return m_AlbumCovers[m_DefaultCoverKey];
}

bool AlbumCoverProvider::hasCover(Song &song)
{
    const QMutexLocker locker{&m_Locker};
    return m_AlbumCovers.contains(song.coverId);
}
