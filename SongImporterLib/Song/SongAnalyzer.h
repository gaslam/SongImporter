#ifndef SONGANALYZER_H
#define SONGANALYZER_H

#include <QObject>
#include "Song.h"

class AlbumCoverProvider;
namespace TagLib{
struct FileRef;
}
class SONGIMPORTERLIB_EXPORT SongAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit SongAnalyzer(const QString& file,AlbumCoverProvider* provider,QObject *parent = nullptr);

public slots:
    void startProcess();

signals:

    void songProcessed(Song song);
    void errorReceived(const QString& error);
private:
    const QString m_FileToProcess;
    AlbumCoverProvider* m_pProvider;

    [[nodiscard]] OperationResult getSongFromFileRef (TagLib::FileRef& file,Song& song);
#if SONGIMPORTERLIB_EXTRACT_ALBUMCOVERS
    void extractAlbumCoverArt(TagLib::FileRef& fileref ,Song& song);
#endif
    void getSongFromFile();
    void getSongFromZip(const QString& zipPath, const QString& filename);
};

#endif // SONGANALYZER_H
