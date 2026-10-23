#include "SongModel.h"

SongModel::SongModel(AlbumCoverProvider* provider,QObject *parent)
    : QAbstractListModel{parent},
    m_AnalyzerManager{provider,parent}
{
    //When the analyzer receives an error, the event must be sent through this class.
    connect(&m_AnalyzerManager,&AnalyzerManager::errorReceived,this,&SongModel::errorReceived);
    //When the analyzer receives a song, the signal must be sent to this class
    connect(&m_AnalyzerManager,&AnalyzerManager::songReceived,this,&SongModel::addSong);
}

int SongModel::columnCount(const QModelIndex &) const
{
    return ContactRoles::ColumnCount;
}

int SongModel::rowCount(const QModelIndex &) const
{
    return static_cast<int>(m_Songs.size());
};


QVariant SongModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Song song{m_Songs.at(index.row())};

    if(!song)
    {
        return QVariant();
    }

    switch (role) {
    case ContactRoles::AlbumCoverRole:
        return song.coverId;
    case ContactRoles::AlbumRole:
        return song.album;
    case ContactRoles::ArtistsRole:
        return song.artists;
    case ContactRoles::TitleRole:
        return song.title;
    case ContactRoles::YearRole:
        return song.year;
    default:
        return QVariant();
    }


}

void SongModel::addSong(Song song)
{
    if(m_Songs.contains(song))
    {
        const QString error{QString{"%1 is already in the list!!"}.arg(song.title)};
        emit errorReceived(error);
        return;
    }

    beginInsertRows(QModelIndex(), m_Songs.size(),m_Songs.size());
    m_Songs.emplace_back(song);
    endInsertRows();
    qDebug() << m_Songs.size() * sizeof(song)
             << "bytes";
    emit songReceived(song);
}

QHash<int, QByteArray> SongModel::roleNames() const
{
    return {
        { YearRole, "year" },
        { ArtistsRole, "artists" },
        { TitleRole, "title" },
        { AlbumCoverRole, "albumCover"},
        { AlbumRole, "album"}
    };
}

void SongModel::addFiles(const QList<QUrl> &files, const QString& destination)
{
    //Runs multithreaded
    m_AnalyzerManager.process(files,destination);
}
