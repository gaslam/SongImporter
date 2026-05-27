#ifndef ANALYZERMANAGER_H
#define ANALYZERMANAGER_H

#include <QObject>

struct Song;
class QuaZipDir;
class SONGIMPORTERLIB_EXPORT AnalyzerManager : public QObject
{
    Q_OBJECT
public:
    explicit AnalyzerManager(QObject *parent = nullptr);

    [[nodiscard]] bool IsStopRequested() const { return m_IsStopRequested;}
    void process(const QString& file);

signals:
    void processStarted();
    void processStopped();
    void errorReceived(const QString& error);
    void songAnalyzed(const Song& song);
private:
    std::atomic<int> m_ActiveTasks{};
    std::atomic<bool> m_IsStopRequested{false};

    void processDirectory(QuaZipDir& dir,
                          const QString& path,
                          const QString& zipPath);
    static void processWorker(AnalyzerManager* analyzerManager, const QString& file);
};

#endif // ANALYZERMANAGER_H
