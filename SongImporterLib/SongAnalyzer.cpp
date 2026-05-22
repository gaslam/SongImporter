#include "SongAnalyzer.h"

#include <FileRef.h>
#include <QFileInfo>
#include <taglib/tpropertymap.h>
#include "Song.h"

using namespace TagLib;

SongAnalyzer::SongAnalyzer(const QString& file, QObject *parent)
    : QObject{parent},
    m_FileToProcess{file}
{}

void SongAnalyzer::startProcess()
{
    FileRef file{m_FileToProcess.toStdString().c_str()};


    if(file.isNull())
    {
        emit errorReceived(QString{"Cannot read file: %1.\nFile might not exist or cannot be read."}.arg(m_FileToProcess));
        return;
    }

    Song song;
    auto result{getSongFromFileRef(file,song)};

    if(!result)
    {
        emit errorReceived(result.errorMessage);
        return;
    }

    emit songProcessed(song,file);

}


OperationResult SongAnalyzer::getSongFromFileRef (const TagLib::FileRef& file,Song& song)
{
    //Get the tag and check if it's valid
    auto tag{file.tag()};

    if(!tag)
    {
        const QString error{QString{"Problem reading information from file: %1.\nFile is empty or cannot be opened."}.arg(m_FileToProcess)};
        emit errorReceived(error);
    }

    //Gather all the data that can be retreived from simple functions.
    //They're guaranteed to be there.
    song.filename = file.file()->name().toString().toCString();
    song.album = tag->album().toCString();

    song.artists = tag->artist().toCString();

    QString title{tag->title().toCString()};
    if(title.isEmpty()) title = QFileInfo{song.filename}.baseName().replace('_', ' ');
    song.title = title;
    song.year = tag->year();

    //Gather all the data that is not always present in an audio file.
    // BPM = Beats Per Minute
    auto properties = file.properties();

    if (properties.contains("BPM")) {
        auto bpmList = properties["BPM"];

        int bpm = bpmList.front().toInt();

        if (bpm > 0)
            song.bpm = bpm;
    }

    //AlbumArtist
    if (properties.contains("ALBUMARTIST")) {
        auto albumArtistList = properties["ALBUMARTIST"];

        if (!albumArtistList.isEmpty()) {
            song.albumArtists = albumArtistList.front().toCString();
        }
    }
    else
    {
        song.albumArtists = song.artists;
    }

    return OperationResult::succeed();
}
