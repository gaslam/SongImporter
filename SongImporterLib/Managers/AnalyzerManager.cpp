#include "AnalyzerManager.h"
#include "../FileUtils.h"
#include "../songAnalyzer.h"

#include <QtConcurrent>
#include <quazipfile.h>
#include <quazipdir.h>

AnalyzerManager::AnalyzerManager(QObject *parent)
    : QObject{parent}
{}

void AnalyzerManager::process(const QString& file)
{
    auto future = QtConcurrent::run([this, file]
                      {
                          if (FileUtils::isNonEmptyZipFile(file))
                              {

                              QuaZip zip(file);

                              if (!zip.open(QuaZip::mdUnzip))
                              {
                                  qWarning() << "Failed to open zip:"
                                             << zip.getZipError();

                                  return;
                              }

                              QuaZipDir root(&zip);

                              processDirectory(root, "",file);
                            return;
                          }
                      });
}

void AnalyzerManager::processWorker(AnalyzerManager* analyzerManager, const QString& file)
{
    QSharedPointer<SongAnalyzer> provider{ new SongAnalyzer{file}};
    SongAnalyzer* analyzer{provider.get()};

    if(analyzerManager->IsStopRequested())
    {
        return;
    }
    connect(analyzer,&SongAnalyzer::songProcessed,analyzerManager,&AnalyzerManager::songAnalyzed);
    connect(analyzer,&SongAnalyzer::errorReceived,analyzerManager,&AnalyzerManager::errorReceived);
    analyzer->startProcess();
}

void AnalyzerManager::processDirectory(QuaZipDir& dir,
                      const QString& path,const QString& zipPath)
{
    QuaZipDir current = dir;

    if (!path.isEmpty())
    {
        if (!current.cd(path))
            return;
    }

    const auto entries =
        current.entryInfoList(QDir::NoDotAndDotDot |
                          QDir::AllEntries);

    const QString currPath{current.path() + "/"};

    for (const auto& entry : entries)
    {
        QString fullPath = currPath + entry.name;

        const auto finalChar{fullPath.length() - 1};
        if (fullPath[finalChar] == '/')
        {
            processDirectory(current, entry.name,zipPath);
        }
        else
        {
            const QString finalFile{QDir{zipPath}.filePath(fullPath)};
            auto future{ QtConcurrent::run(AnalyzerManager::processWorker, this,finalFile)};
            m_ActiveTasks++;
        }
    }
}
