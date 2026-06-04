#ifndef ALBUMCOVERPROVIDER_H
#define ALBUMCOVERPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QImage>
#include <QMutex>

struct Song;
class SONGIMPORTERLIB_EXPORT AlbumCoverProvider : public QQuickImageProvider
{
    Q_OBJECT
public:
    explicit AlbumCoverProvider();

    void setImage(Song& song,const QImage &image);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

private:
    QHash<QString,QImage> m_AlbumCovers;
    QString m_DefaultCoverKey;
    QMutex m_Locker;

    void extractAlbumCoverArt(const QString& file, const QString& id, Song& song);
};

#endif // ALBUMCOVERPROVIDER_H
