#ifndef ANALYZERMANAGER_H
#define ANALYZERMANAGER_H

#include "Utils/BytesTracker.h"
#include <QFuture>
#include <QFutureWatcher>
#include "Song/Song.h"
class QuaZipDir;
class AlbumCoverProvider;
class SONGIMPORTERLIB_EXPORT AnalyzerManager : public QObject
{
    Q_OBJECT
public:
    AnalyzerManager(AlbumCoverProvider* albumCoverProvider,QObject *parent = nullptr);

    [[nodiscard]] AlbumCoverProvider* getAlbumCoverProvider() const {return m_AlbumCoverProvider;}
    void process(const QList<QUrl>& files, const QString& destination);

signals:
    void processStarted();
    void processStopped();
    void errorReceived(const QString& error);
    void songReceived(Song song);

public slots:
    [[nodiscard]] bool isStopRequested() const { return m_IsStopRequested.load();}
    void requestStop() noexcept;

private:
    std::atomic<bool> m_IsStopRequested{false};
    std::atomic<qsizetype> m_ActiveTasks{};
    BytesTracker m_BytesTracker;
    QMutex m_StarterWatchersMutex;
    QMutex m_WorkerWatchersMutex;
    QVector<QFutureWatcher<void>*> m_StarterWatchers;
    QVector<QFutureWatcher<void>*> m_WorkerWatchers;
    AlbumCoverProvider* m_AlbumCoverProvider{};

    void addWorker(const QString& file);
    void removeWorker(QFutureWatcher<void>* watcher);
    void processDirectory(QuaZipDir& dir,
                          const QString& path,
                          const QString& zipPath);
    static void processWorker(AnalyzerManager* analyzerManager,QFutureWatcher<void>* watcher, const QString& file);
};

#endif // ANALYZERMANAGER_H
