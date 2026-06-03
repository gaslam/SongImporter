#ifndef ALBUMCOVERPROVIDER_H
#define ALBUMCOVERPROVIDER_H

#include <QObject>

class AlbumCoverProvider : public QObject
{
    Q_OBJECT
public:
    explicit AlbumCoverProvider(QObject *parent = nullptr);

signals:
};

#endif // ALBUMCOVERPROVIDER_H
