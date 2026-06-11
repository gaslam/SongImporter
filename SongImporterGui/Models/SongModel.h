#ifndef SONGMODEL_H
#define SONGMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>

class SongModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit SongModel(QObject *parent = nullptr);
};

#endif // SONGMODEL_H
