#ifndef SONGANALYZER_H
#define SONGANALYZER_H

#include <QObject>

struct Song;
namespace TagLib{
 class FileRef;
}
class SONGIMPORTERLIB_EXPORT SongAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit SongAnalyzer(const QString& file,QObject *parent = nullptr);

public slots:
    void startProcess();

signals:

    void songProcessed(const Song& song, const TagLib::FileRef& ref);
    void errorReceived(const QString& error);
private:
    const QString m_FileToProcess;

    [[nodiscard]] OperationResult getSongFromFileRef (const TagLib::FileRef& file,Song& song);
};

#endif // SONGANALYZER_H
