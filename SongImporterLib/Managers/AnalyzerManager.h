#ifndef ANALYZERMANAGER_H
#define ANALYZERMANAGER_H

#include "../BytesTracker.h"
#include "../Song.h"
class QuaZipDir;
class AlbumCoverProvider;
class SONGIMPORTERLIB_EXPORT AnalyzerManager : public QObject
{
    Q_OBJECT
public:
    AnalyzerManager(AlbumCoverProvider* albumCoverProvider,QObject *parent = nullptr);

    [[nodiscard]] bool IsStopRequested() const { return m_IsStopRequested;}
    [[nodiscard]] AlbumCoverProvider* getAlbumCoverProvider() const {return m_AlbumCoverProvider;}
    void process(const QList<QUrl>& files, const QString& destination);

signals:
    void processStarted();
    void processStopped();
    void errorReceived(const QString& error);

private slots:

    void songReceived(Song song);
    void updateActiveTasks();
private:
    BytesTracker m_BytesTracker;
    std::atomic<int> m_ActiveTasks{};
    std::atomic<bool> m_IsStopRequested{false};
    AlbumCoverProvider* m_AlbumCoverProvider{};

    void addWorker(const QString& file);
    void processDirectory(QuaZipDir& dir,
                          const QString& path,
                          const QString& zipPath);
    static void processWorker(AnalyzerManager* analyzerManager, const QString& file);
};

#endif // ANALYZERMANAGER_H
