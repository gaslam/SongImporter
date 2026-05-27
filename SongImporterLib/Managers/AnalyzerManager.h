#ifndef ANALYZERMANAGER_H
#define ANALYZERMANAGER_H

#include <QObject>

struct Song;
class AnalyzerManager : public QObject
{
    Q_OBJECT
public:
    explicit AnalyzerManager(QObject *parent = nullptr);

signals:
    void processStarted();
    void processStopped();
    void errorReceived(const QString& error);
    void songAnalyzed(const Song& song);
private:
    std::atomic<int> m_ActiveTasks{};
    std::atomic<bool> m_IsStopRequested{false};
};

#endif // ANALYZERMANAGER_H
