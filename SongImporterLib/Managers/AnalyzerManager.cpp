#include "AnalyzerManager.h"
#include "Utils/SoftwareUtils.h"
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

    /// Start multithreaded process to analyze the file.
    QFutureWatcher<void>* watcher = new QFutureWatcher<void>{};
    auto future{QtConcurrent::run([this, files,destination]()
                                  {
                                    /// Check if enough space is left on the destination hard drive
                                      auto result{m_BytesTracker.checkSpaceAvailable(files,destination)};
                                      if(!result)
                                      {
                                          emit errorReceived(result.errorMessage);
                                          return;
                                      }

                                      /// If stop requested, bail out early
                                      if (isStopRequested()) return;

                                      auto supportedFiles{SoftwareUtils::supportedAudioFormatsString(SoftwareUtils::Rekordbox)};

                                      foreach (auto& url, files) {
                                          if(m_IsStopRequested)
                                          {
                                              return;
                                          }
                                          QString file = url.path();
                                          if(url.isLocalFile())
                                          {
                                              //Qt can get annoying if the file url is incorrect
                                              file = url.toLocalFile();
                                          }

                                          //If file is a zip file, cycly through all files
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

                                              processDirectory(root,file,supportedFiles);
                                          }
                                          else
                                          {
                                              addWorker(file);
                                          }
                                      }
                                  })};

    watcher->setFuture(future);
    if(m_Watchers.isEmpty())
    {
        emit processStarted();
    }
    {
            QMutexLocker locker{&m_WatchersMutex};
            m_Watchers.append(watcher);
    }
    /// Remove the watcher from the tracked workers when processing finishes.
    connect(watcher, &QFutureWatcher<void>::finished, [this, watcher]() {
        removeWorker(watcher);
    });
    connect(watcher, &QFutureWatcher<void>::canceled, [this, watcher]() {
        removeWorker(watcher);
    });


}

void AnalyzerManager::requestStop() noexcept
{
    m_IsStopRequested.store(true);
        foreach (auto future, m_Watchers) {
            future->canceled();
        }

        m_Watchers.empty();
    emit processStopped();
}

void AnalyzerManager::addWorker(const QString& file)
{

    auto *watcher = new QFutureWatcher<void>();
    QEventLoop wait;


    QObject::connect(watcher, &QFutureWatcher<void>::finished, &wait, &QEventLoop::quit );
    /// Remove the watcher from the tracked workers when processing finishes.
    connect(watcher, &QFutureWatcher<void>::finished, [this, watcher]() {
        removeWorker(watcher);
    });

    connect(watcher, &QFutureWatcher<void>::canceled, [this, watcher]() {
        removeWorker(watcher);
    });

    //Run the process in a multithreaded fashion.
    QFuture<void> future = QtConcurrent::run(AnalyzerManager::processWorker, this,watcher, file);
    watcher->setFuture(future);
    {
            m_Watchers.append(watcher);
            QMutexLocker locker{&m_WatchersMutex};
    }
}


void AnalyzerManager::processWorker(AnalyzerManager* analyzerManager,QFutureWatcher<void>* watcher, const QString& file)
{
    /// Run the process in a multithreaded fashion.
    /// Analyzer is of type QSharedPointer to release memory, when the analyzer is finished
    /// The object must be alive, until it finishes.
    /// Using local variables causes segmentation faults
    QSharedPointer<SongAnalyzer> analyzer{new SongAnalyzer{file,analyzerManager->getAlbumCoverProvider()}};
    SongAnalyzer* pAnalyzer{analyzer.get()};

    if(analyzerManager->isStopRequested())
    {
        return;
    }
    connect(pAnalyzer,&SongAnalyzer::songProcessed,analyzerManager,&AnalyzerManager::songReceived);
    connect(pAnalyzer,&SongAnalyzer::errorReceived,analyzerManager,&AnalyzerManager::errorReceived);
    analyzer->startProcess();
    watcher->finished();
}

void AnalyzerManager::removeWorker(QFutureWatcher<void>* watcher)
{
    if(!watcher)
    {
        return;
    }
    {
        QMutexLocker locker(&m_WatchersMutex);
        m_Watchers.erase(std::remove(m_Watchers.begin(), m_Watchers.end(), watcher), m_Watchers.end());
        watcher->deleteLater();
    }

    if(m_Watchers.isEmpty())
    {
        emit processStopped();
    }
}

void AnalyzerManager::processDirectory(QuaZipDir& dir,
                                       const QString& zipPath, const QList<QString>& supportedFiles)
{
    /// Abort directory traversal if cancellation has been requested.
    if(isStopRequested()) return;

    /// process audio files
    const auto files =
        dir.entryInfoList(
            QDir::Files | QDir::NoDotAndDotDot);

    const QString originPath{dir.path()};
    const QString currPath{originPath.isEmpty() ? "" : originPath + '/'};

    for (const auto& entry : files)
    {
        if(isStopRequested()) return;
        const QString finalFile =
            zipPath + '/' + currPath + entry.name;

        addWorker(finalFile);
    }

    /// recurse directories first
    const auto dirs =
        dir.entryInfoList(
            QStringList(),
            QDir::Dirs | QDir::NoDotAndDotDot);

    /// Iterate the subdirectories using recursion.

    for (const auto& entry : dirs)
    {
        if(isStopRequested()) return;
        QuaZipDir entryPath{dir};
        if(!entryPath.cd(entry.name)){
            continue;
        }
        processDirectory(entryPath,zipPath,supportedFiles);
    }
}
