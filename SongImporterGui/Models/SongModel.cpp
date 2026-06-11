#include "SongModel.h"

SongModel::SongModel(AlbumCoverProvider* provider,QObject *parent)
    : QAbstractListModel{parent},
    m_AnalyzerManager{provider,parent}
{
    connect(&m_AnalyzerManager,&AnalyzerManager::errorReceived,this,&SongModel::errorReceived);
    connect(&m_AnalyzerManager,&AnalyzerManager::songReceived,this,&SongModel::addSong);
}

int SongModel::columnCount(const QModelIndex &) const
{
    return ContactRoles::ColumnCount;
}

int SongModel::rowCount(const QModelIndex &) const
{
    return static_cast<int>(m_Songs.count());
};


QVariant SongModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const Song& song{m_Songs.at(index.row())};

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
        return;
    }
    beginInsertRows(QModelIndex(), m_Songs.size(),m_Songs.size());
    m_Songs << song;

    endInsertRows();
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
    m_AnalyzerManager.process(files,destination);
}
