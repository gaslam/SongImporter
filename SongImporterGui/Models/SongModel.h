#ifndef SONGMODEL_H
#define SONGMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>
#include <Song/Song.h>
#include <Managers/AnalyzerManager.h>

class AlbumCoverProvider;

/// Model that exposes song metadata to Qt's model/view framework.
class SongModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SongModel(AlbumCoverProvider* provider, QObject *parent = nullptr);

    ///Used to expose songData to views and QML
    enum ContactRoles {
        TitleRole = Qt::UserRole + 1,
        ArtistsRole,
        YearRole,
        AlbumCoverRole,
        AlbumRole,
        ColumnCount
    };

    /// Returns the number of columns for the given parent index.
    int columnCount(const QModelIndex& parent) const;
    /// Returns the number of rows for the given parent index.
    int rowCount(const QModelIndex &) const;
    //Determines the type of data that should be returned, based on the display role
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
public slots:
    //Add a file and immediately starts the analyses.
    void addFiles(const QList<QUrl>& files, const QString& destination);
signals:
    void errorReceived(const QString& error);
    //Sends the song
    void songReceived(Song song);
private slots:
    void addSong(Song song);
private:
    //A helper class used to manage all instances of AnalyzerManager.
    AnalyzerManager m_AnalyzerManager;
    QList<Song> m_Songs{};
};

#endif // SONGMODEL_H
