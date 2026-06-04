#ifndef ALBUMCOVERPROVIDER_H
#define ALBUMCOVERPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QImage>
#include <QMutex>
#include "../Song.h"

class SONGIMPORTERLIB_EXPORT AlbumCoverProvider : public QQuickImageProvider
{
    Q_OBJECT
public:
    explicit AlbumCoverProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
    bool hasCover(Song& song);

public slots:
    void setImage(Song song);
signals:
    void albumCoverAdded(Song song);

private:
    QHash<QString,QImage> m_AlbumCovers;
    QString m_DefaultCoverKey;
    QMutex m_Locker;

    void extractAlbumCoverArt(const QString& file, const QString& id, Song& song);
};

#endif // ALBUMCOVERPROVIDER_H
