#include "AlbumCoverProvider.h"
#include "../Song.h"
#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/id3v2tag.h>

using namespace TagLib;

AlbumCoverProvider::AlbumCoverProvider()
    : QQuickImageProvider{QQuickImageProvider::Image}
{}

void AlbumCoverProvider::setImage(Song& song,const QImage &image)
{
    QString songId{QString("%1_%2").arg(song.albumArtists.toLower(),song.album.toLower())};
    if(hasCover(song))
    {
        song.coverId = songId;
        return;
    }

    extractAlbumCoverArt(song.filename,songId,song);
}

void AlbumCoverProvider::extractAlbumCoverArt(const QString& filePath, const QString& id, Song& song)
{
    FileRef fileref{filePath.toStdString().c_str()};
    File* file{fileref.file()};

    if(!file)
    {
        return;
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
