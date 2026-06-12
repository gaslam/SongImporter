#ifndef SONGMODEL_H
#define SONGMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>
#include <Song/Song.h>
#include <Managers/AnalyzerManager.h>

class AlbumCoverProvider;
class SongModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SongModel(AlbumCoverProvider* provider, QObject *parent = nullptr);

    enum ContactRoles {
        TitleRole = Qt::UserRole + 1,
        ArtistsRole,
        YearRole,
        AlbumCoverRole,
        AlbumRole,
        ColumnCount
    };

    // QAbstractItemModel interface
    int columnCount(const QModelIndex& parent) const;
    int rowCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
public slots:
    void addFiles(const QList<QUrl>& files, const QString& destination);
signals:
    void errorReceived(const QString& error);
private slots:
    void addSong(Song song);
private:
    AnalyzerManager m_AnalyzerManager;
    QList<Song> m_Songs{};
};

#endif // SONGMODEL_H
