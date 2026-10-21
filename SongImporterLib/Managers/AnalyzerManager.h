#ifndef ANALYZERMANAGER_H
#define ANALYZERMANAGER_H

#include "Utils/BytesTracker.h"
#include <QFuture>
#include <QFutureWatcher>
#include "Song/Song.h"
class QuaZipDir;
class AlbumCoverProvider;

///A class used to manage all instances of SongAnalyzer
/// It tracks multiple threads and passes songs through signals.
class SONGIMPORTERLIB_EXPORT AnalyzerManager : public QObject
{
    Q_OBJECT
public:
    AnalyzerManager(AlbumCoverProvider* albumCoverProvider,QObject *parent = nullptr);

    ///This function is intended to retrieve the CoverProvider across multiple threads.
    /// See the function Add worker.
    [[nodiscard]] AlbumCoverProvider* getAlbumCoverProvider() const {return m_AlbumCoverProvider;}

    ///Proccesses a batch of files selected by the user.
    /// \param files Url's of files to import.
    /// \param destination The future destination of the files. Used to check storage of the destinations hard drive.
    void process(const QList<QUrl>& files, const QString& destination);

signals:
    ///Signals when a process has started.
    /// When the user drops a file and no other files are processing, the process starts.
    void processStarted();
    ///Signals when a process has stopped.
    /// When all files are processed, it stops.
    void processStopped();
    ///Signals when an error has occurred.
    /// \param error The error message that is being passed.
    void errorReceived(const QString& error);
    ///Signals when an song was analyzed.
    /// \param song A pointer to the song that was analyzed.
    void songReceived(Song* song);

public slots:
        ///Checks if a stop was requested for the process of analyzing a song.
    [[nodiscard]] bool isStopRequested() const { return m_IsStopRequested.load();}
    ///Requests a song by signalling that the process must be stopped.
    void requestStop() noexcept;

private:

    /// When the application gets aborted, a stop gets requested
    /// Simultaneous reads and writes to this variable could cause a data race.
    /// It doesn't cause any problem's, so I decided to go with an atomic
    std::atomic<bool> m_IsStopRequested{false};
    BytesTracker m_BytesTracker;
    /// Keeps track of all the threads. When a thread is finished, it gets removed
    QMutex m_WatchersMutex;
    QVector<QFutureWatcher<void>*> m_Watchers;

    /// Required to add an album cover
    /// Maybe this could be improved
    /// TODO: Maybe move this to SongImporterGui. Only guis and no console application will need it.
    AlbumCoverProvider* m_AlbumCoverProvider{};

    /// Adds a worker to m_Watchers and immediately starts the process to analyze a song on a new thread.
    /// \param file The file that must be analyzed.
    void addWorker(const QString& file);
    /// Removes a worker from m_Watchers.
    /// \param watcher The watcher that must be removed.
    void removeWorker(QFutureWatcher<void>* watcher);

    /// Processes the zip directory and subidirectories. It will call itself using recursion
    /// \param dir the zip directory
    /// \param zipPath the zip directory in string
    /// \param supportedFiles A list of supported files. Must be in string due to how QuaZip works.
    void processDirectory(QuaZipDir& dir,
                          const QString& zipPath,
                           const QList<QString>& supportedFiles);
    /// Processes the zip directory and subidirectories. It will call itself using recursion
    /// \param dir the zip directory
    /// \param zipPath the zip directory in string
    /// \param supportedFiles A list of supported files. Must be in string due to how QuaZip works.
    static void processWorker(AnalyzerManager* analyzerManager,QFutureWatcher<void>* watcher, const QString& file);
};

#endif // ANALYZERMANAGER_H
