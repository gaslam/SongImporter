#ifndef SONGANALYZER_H
#define SONGANALYZER_H

#include <QObject>

class SONGIMPORTERLIB_EXPORT SongAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit SongAnalyzer(const QString& file,QObject *parent = nullptr);

signals:

private:
    const QString m_FileToProcess;
};

#endif // SONGANALYZER_H
