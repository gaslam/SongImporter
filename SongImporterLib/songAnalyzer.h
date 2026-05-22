#ifndef SONGANALYZER_H
#define SONGANALYZER_H

#include <QObject>

class SongAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit SongAnalyzer(QObject *parent = nullptr);

signals:
};

#endif // SONGANALYZER_H
