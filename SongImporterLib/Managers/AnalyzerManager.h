#ifndef ANALYZERMANAGER_H
#define ANALYZERMANAGER_H

#include <QObject>

class AnalyzerManager : public QObject
{
    Q_OBJECT
public:
    explicit AnalyzerManager(QObject *parent = nullptr);

signals:

private:
    std::atomic<int> m_ActiveTasks{};
    std::atomic<bool> m_StopRequested{false};
};

#endif // ANALYZERMANAGER_H
