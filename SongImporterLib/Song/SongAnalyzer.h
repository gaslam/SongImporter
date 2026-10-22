#ifndef SONGANALYZER_H
#define SONGANALYZER_H

#include <QObject>
#include "Song.h"


/// Class declaration for AlbumCoverProvider and FileRef inside TagLib namespace
class AlbumCoverProvider;
namespace TagLib{
struct FileRef;
}

/// Analyzes audio files and extracts their metadata.
class SONGIMPORTERLIB_EXPORT SongAnalyzer : public QObject
{
    Q_OBJECT
public:
    /// Constructs a SongAnalyzer for the specified file.
    /// \param file Path to the audio file to analyze.
    /// \param provider Provider used to handle album cover artwork.
    /// \param parent Optional parent QObject that manages this object's lifetime.
    explicit SongAnalyzer(const QString& file,AlbumCoverProvider* provider,QObject *parent = nullptr);

public slots:
    /// Starts processing the specified file and extracting its metadata.
    void startProcess();

signals:

    /// Emitted when a song has been successfully processed.
    /// \param song Pointer to the processed song.
    void songProcessed(Song* song);

    /// Emitted when an error occurs during file processing.
    /// \param error Description of the error.
    void errorReceived(const QString& error);
private:
    /// Path of the file to process. Cannot be reassigned after construction.
    const QString m_FileToProcess;
    /// Pointer to the AlbumCoverProvider that will return album covers
    AlbumCoverProvider* m_pProvider;

    /// Extracts the song from a TagLib file reference.
    /// \param file A taglib file reference to the audio file being analyzed.
    /// \param song A pointer to the song that is updated with all the data.
    /// \returns The result of the operation.
    [[nodiscard]] OperationResult getSongFromFileRef (TagLib::FileRef& file,Song* song);
#if SONGIMPORTERLIB_EXTRACT_ALBUMCOVERS
    /// Extracts album cover artwork from the file and associates it with the song.
    /// \param fileref Reference to the audio file being analyzed.
    /// \param song Pointer to the Song object that receives the album cover information
    void extractAlbumCoverArt(TagLib::FileRef& fileref ,Song* song);
#endif
    void getSongFromFile();
    void getSongFromZip(const QString& zipPath, const QString& filename);
};

#endif // SONGANALYZER_H
