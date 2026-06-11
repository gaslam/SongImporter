#ifndef ALBUMCOVERPROVIDER_H
#define ALBUMCOVERPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QImage>
#include <QMutex>
#include "../Song.h"

namespace TagLib{
class FileRef;
}

class SONGIMPORTERLIB_EXPORT AlbumCoverProvider : public QQuickImageProvider
{
    Q_OBJECT
public:
    explicit AlbumCoverProvider(QString& defaultId,QImage& defaultLogo);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
    bool hasCover(Song& song);

    void setImage(Song& song,TagLib::FileRef& fileref);

private:
    QHash<QString,QImage> m_AlbumCovers;
    QString m_DefaultCoverKey;
    QMutex m_Locker;

    void extractAlbumCoverArt(TagLib::FileRef& fileref, const QString& id, Song& song);
};

#endif // ALBUMCOVERPROVIDER_H
