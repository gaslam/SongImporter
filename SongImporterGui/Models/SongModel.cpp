#include "SongModel.h"

SongModel::SongModel(AlbumCoverProvider* provider,QObject *parent)
    : QAbstractListModel{parent},
    m_AnalyzerManager{provider,parent}
{
    /// When the analyzer receives an error, the event must be sent through this class.
    connect(&m_AnalyzerManager,&AnalyzerManager::errorReceived,this,&SongModel::errorReceived);
    /// When the analyzer receives a song, the signal must be sent to this class
    connect(&m_AnalyzerManager,&AnalyzerManager::songReceived,this,&SongModel::addSong);
    /// Receives a signal when the copying or analysis of the song has started.
    connect(&m_AnalyzerManager,&AnalyzerManager::processStarted,this,&SongModel::processStarted);
    /// Receives a signal when the copying or analysis of the song, has stopped.
    connect(&m_AnalyzerManager,&AnalyzerManager::processStopped,this,&SongModel::processStopped);
}

int SongModel::columnCount(const QModelIndex &) const
{
    return ContactRoles::ColumnCount;
}

int SongModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return static_cast<int>(m_Songs.size());
}


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

void SongModel::removeSongs(const QVariantList &indexes)
{
    /// Get the latest index.
    long long i{indexes.count() - 1};

    do {
        bool isIndex{};
        /// Convert variant to int
        const int currId{indexes[i].toInt(&isIndex)};

        if(!isIndex)
        {
            continue;
        }


        /// Check through model index if index is valid.
        QModelIndex modelIndex { index(currId, 0)};

        if (!modelIndex.isValid())
            continue;

        /// Create a new model index.
        /// The previous model index does not work, so I had to make a new one.
        /// I don't know why.
        beginRemoveRows(QModelIndex{}, currId, currId);
        /// Delete the indexes in descending order.
        m_Songs.erase(m_Songs.begin() + currId);
        endRemoveRows();
        --i;

    } while (i >= 0);
}
