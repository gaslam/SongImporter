#include "SongAnalyzer.h"

#include <FileRef.h>
#include <QFileInfo>
#include <QuaZip.h>
#include <QuaZipFile.h>
#include <taglib/tpropertymap.h>
#include <taglib/tbytevectorstream.h>
#include "Providers/AlbumCoverProvider.h"

using namespace TagLib;

SongAnalyzer::SongAnalyzer(const QString& file, AlbumCoverProvider* provider, QObject *parent)
    : QObject{parent},
    m_FileToProcess{file},
    m_pProvider{provider}
{}

void SongAnalyzer::startProcess()
{
    const QString zipExt{".zip"};
    int split = m_FileToProcess.indexOf(zipExt);
    //Checks if the filepath is a zip file
    if (split != -1)
    {
        split += zipExt.length();

        QString zipPath{ m_FileToProcess.left(split)};
        QString fileName { m_FileToProcess.mid(split + 1)};
        getSongFromZip(zipPath,fileName);
    }
    else
    {
        getSongFromFile();
    }


}

void SongAnalyzer::getSongFromFile()
{
    FileRef file {m_FileToProcess.toStdString().c_str()};
    if(file.isNull())
    {
        const QString error{QString{"Cannot read file: %1.\nFile might not exist or cannot be read."}.arg(m_FileToProcess)};
        emit errorReceived(error);
        return;
    }
    Song song;
    auto result{getSongFromFileRef(file,song)};

    if(!result)
    {
        emit errorReceived(result.errorMessage);
        return;
    }
    emit songProcessed(song);
}

void SongAnalyzer::getSongFromZip(const QString& zipPath, const QString& filename)
{
    //TODO: try to make this more efficient
    //If not, make this more readable
    // Open the zip
    QuaZip zip(zipPath);

    if (!zip.open(QuaZip::mdUnzip)) {

        const QString error{ "Cannot open ZIP archive: " + zipPath};
        emit errorReceived(error);
        return;
    }

    if (!zip.setCurrentFile(filename))
    {
        const QString error{ QString{"File: %1 not found inside zip: %2"}.arg(zipPath,filename)};
        emit errorReceived(error);
        return;
    }

    // Open the zip file
    QuaZipFile zipFile(&zip);

    if (!zipFile.open(QIODevice::ReadOnly)) {
        const QString error{ "Failed to open:" + filename};
        emit errorReceived(error);
        return;
    }

    QByteArray data{zipFile.readAll()};
    //TODO: try to make this more efficient
    try{
        TagLib::ByteVector bv(data.constData(), data.size());
        TagLib::ByteVectorStream memStream(bv);
        FileRef file {&memStream};
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
        song.filename = m_FileToProcess;
        emit songProcessed(song);

    }catch (std::exception& e){
        const QString error{ QString{"%1 for file: %2"}.arg(e.what(),zipPath)};
        emit errorReceived(error);
        return;
    }
    zipFile.close();
}


OperationResult SongAnalyzer::getSongFromFileRef (TagLib::FileRef& file,Song& song)
{
    //Get the tag and check if it's valid
    auto tag{file.tag()};

    if(tag->isEmpty())
    {
#if SONGIMPORTERLIB_EXTRACT_ALBUMCOVERS
        if(m_pProvider)
        {
            m_pProvider->setImage(song,file);
        }
#endif
        return OperationResult::succeed();
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

#if SONGIMPORTERLIB_EXTRACT_ALBUMCOVERS
    if(m_pProvider)
    {
        m_pProvider->setImage(song,file);
    }
#endif

    return OperationResult::succeed();
}
