#include "AnalyzerManager.h"
#include "Utils/FileUtils.h"
#include "Song/SongAnalyzer.h"

#include <QtConcurrent>
#include <quazipfile.h>
#include <quazipdir.h>
#include "Providers/AlbumCoverProvider.h"

AnalyzerManager::AnalyzerManager(AlbumCoverProvider* provider,QObject *parent)
    : QObject{parent},
    m_AlbumCoverProvider{provider}
{}

void AnalyzerManager::process(const QList<QUrl>& files, const QString& destination)
{
    int prev = m_ActiveTasks.exchange(files.length() + m_ActiveTasks);
    if(prev == 0)
    {
        emit processStarted();
    }

    QFutureWatcher<void>* watcher = new QFutureWatcher<void>{};
    auto future{QtConcurrent::run([this, files,destination]()
                                  {
                                      auto result{m_BytesTracker.checkSpaceAvailable(files,destination)};
                                      if(!result)
                                      {
                                          emit errorReceived(result.errorMessage);
                                          return;
                                      }

                                      // If stop requested before we start, bail out early
                                      if (isStopRequested()) return;

                                      foreach (auto& url, files) {
                                          if(m_IsStopRequested)
                                          {
                                              return;
                                          }
                                          QString file = url.path();
                                          if(url.isLocalFile())
                                          {
                                              file = url.toLocalFile();
                                          }

                                          if (FileUtils::isNonEmptyZipFile(file))
                                          {

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

    watcher->setFuture(future);
    m_StarterWatchers.append(watcher);
}

void AnalyzerManager::requestStop() noexcept
{
    m_IsStopRequested.store(true);
    {
        foreach (auto future, m_StarterWatchers) {
            future->canceled();
        }

        m_StarterWatchers.empty();
    }

    {
        foreach (auto future, m_WorkerWatchers) {
            future->canceled();
        }

        m_WorkerWatchers.empty();
    }
    m_ActiveTasks.store(0);
    emit processStopped();
}

void AnalyzerManager::addWorker(const QString& file)
{
    QMutexLocker locker{&m_WorkerWatchersMutex};

    auto *watcher = new QFutureWatcher<void>();

    connect(watcher, &QFutureWatcher<void>::finished, [this, watcher]() {
        removeWorker(watcher);
    });

    connect(watcher, &QFutureWatcher<void>::canceled, [this, watcher]() {
        removeWorker(watcher);
    });

    QFuture<void> future = QtConcurrent::run(AnalyzerManager::processWorker, this,watcher, file);
    watcher->setFuture(future);

    m_WorkerWatchers.append(watcher);
}


void AnalyzerManager::processWorker(AnalyzerManager* analyzerManager,QFutureWatcher<void>* watcher, const QString& file)
{
    QSharedPointer<SongAnalyzer> provider{ new SongAnalyzer{file,analyzerManager->getAlbumCoverProvider()}};
    SongAnalyzer* analyzer{provider.get()};

    if(analyzerManager->isStopRequested())
    {
        return;
    }
    connect(analyzer,&SongAnalyzer::songProcessed,analyzerManager,&AnalyzerManager::songReceived);
    connect(analyzer,&SongAnalyzer::errorReceived,analyzerManager,&AnalyzerManager::errorReceived);
    analyzer->startProcess();
    watcher->finished();
}

void AnalyzerManager::removeWorker(QFutureWatcher<void>* watcher)
{
    if(!watcher)
    {
        return;
    }
    QMutexLocker locker(&m_WorkerWatchersMutex);
    m_WorkerWatchers.erase(std::remove(m_WorkerWatchers.begin(), m_WorkerWatchers.end(), watcher), m_WorkerWatchers.end());
    watcher->deleteLater();
    --m_ActiveTasks;
    if (m_ActiveTasks == 0)
        emit processStopped();
}

void AnalyzerManager::processDirectory(QuaZipDir& dir,
                                       const QString& path,const QString& zipPath)
{
    if(isStopRequested()) return;
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
        if(isStopRequested()) return;
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
        if(isStopRequested()) return;
        processDirectory(current, entry.name, zipPath);
    }
}
