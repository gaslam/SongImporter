#include "AnalyzerManager.h"
#include "../FileUtils.h"
#include "../songAnalyzer.h"

#include <QtConcurrent>
#include <quazipfile.h>
#include <quazipdir.h>

AnalyzerManager::AnalyzerManager(QObject *parent)
    : QObject{parent}
{}

void AnalyzerManager::process(const QList<QUrl>& files, const QString& destination)
{
    auto future {
                QtConcurrent::run([this, files,destination]
                                  ()
                                  {
                                      auto result{m_BytesTracker.checkSpaceAvailable(files,destination)};
                                      if(!result)
                                      {
                                          emit errorReceived(result.errorMessage);
                                          return;
                                      }

                                      foreach (auto& url, files) {
                                          QString file = url.path();
                                          if(url.isLocalFile())
                                          {
                                              file = url.toLocalFile();
                                          }

                                          if (FileUtils::isNonEmptyZipFile(file))
                                          {

                                              if(m_IsStopRequested)
                                              {
                                                  return;
                                              }

                                              QuaZip zip(file);

                                              if (!zip.open(QuaZip::mdUnzip))
                                              {
                                                  const QString errorMessage{"Failed to open zip: " + file};
                                                  emit errorReceived(errorMessage);

                                                  continue;
                                              }

                                              QuaZipDir root(&zip);

                                              processDirectory(root, "",file);
                                          }
                                          else
                                          {
                                              addWorker(file);
                                          }
                                      }
                                  })};
}

void AnalyzerManager::addWorker(const QString& file)
{
    auto future{ QtConcurrent::run(AnalyzerManager::processWorker, this,file)};
    if(m_ActiveTasks == 0)
    {
        emit processStarted();
    }
    m_ActiveTasks++;
}

void AnalyzerManager::songReceived(const Song &)
{
    updateActiveTasks();
}

void AnalyzerManager::updateActiveTasks()
{
    --m_ActiveTasks;

    if(m_ActiveTasks == 0)
    {
        emit processStopped();
    }
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
    connect(analyzer,&SongAnalyzer::songProcessed,analyzerManager,&AnalyzerManager::songReceived);
    connect(analyzer,&SongAnalyzer::errorReceived,analyzerManager,&AnalyzerManager::errorReceived);
    connect(analyzer,&SongAnalyzer::errorReceived,analyzerManager,&AnalyzerManager::updateActiveTasks);
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

    // process audio files
    const auto files =
        current.entryInfoList(
            QStringList() << "*.mp3"
                          << "*.flac",
            QDir::Files | QDir::NoDotAndDotDot);

    const QString originPath{current.path()};
    const QString currPath{originPath.isEmpty() ? "" : originPath + '/'};

    for (const auto& entry : files)
    {
        const QString finalFile =
            zipPath + '/' + currPath + entry.name;

        addWorker(finalFile);
    }

    // recurse directories first
    const auto dirs =
        current.entryInfoList(
            QStringList(),
            QDir::Dirs | QDir::NoDotAndDotDot);

    for (const auto& entry : dirs)
    {
        processDirectory(current, entry.name, zipPath);
    }
}
