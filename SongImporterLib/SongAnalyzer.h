#ifndef SONGANALYZER_H
#define SONGANALYZER_H

#include <QObject>

struct Song;
namespace TagLib{
struct FileRef;
}
struct SongEventWrapper;
class SONGIMPORTERLIB_EXPORT SongAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit SongAnalyzer(const QString& file,QObject *parent = nullptr);

public slots:
    void startProcess();

signals:

    void songProcessed(const SongEventWrapper& wrapper);
    void errorReceived(const QString& error);
private:
    const QString m_FileToProcess;

    [[nodiscard]] OperationResult getSongFromFileRef (const TagLib::FileRef& file,Song& song);
    void getSongFromFile();
    void getSongFromZip(const QString& zipPath, const QString& filename);
};

#endif // SONGANALYZER_H
