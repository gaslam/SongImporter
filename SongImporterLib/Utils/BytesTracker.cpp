#include "BytesTracker.h"
#include <QUrl>
#include <QFileInfo>
#include <QuaZip.h>
#include <QStorageInfo>
#include "FileUtils.h"

/// Checks whether sufficient disk space is available for the specified files.
OperationResult BytesTracker::checkSpaceAvailable(const QList<QUrl>& files, const QString& destination)
{
    // Prevent concurrent access to the total tracked file size.
    QMutexLocker locker(&m_Mutex);

    qint64 size{};

    // Calculate the total size of the files to be added.
    foreach (const QUrl& file, files)
    {
        if (FileUtils::isNonEmptyZipFile(file))
        {
            // Open the ZIP archive to calculate the size of its extracted contents.
            QuaZip zip{file.toString()};

            if (!zip.open(QuaZip::mdUnzip))
                continue;

            // Sum the uncompressed sizes of all files in the archive.
            for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile())
            {
                QuaZipFileInfo zipInfo;
                zip.getCurrentFileInfo(&zipInfo);

                size += zipInfo.uncompressedSize;
            }

            zip.close();
        }
        else
        {
            // Add the size of a regular file.
            QFileInfo info{file.toString()};
            size += info.size();
        }
    }

    // Retrieve the available space on the destination's storage device.
    const QStorageInfo storage{destination};
    const qint64 bytesAv{storage.bytesAvailable()};

    // Include the sizes of files already accounted for by this tracker.
    const qint64 newSize{size + m_TotalBytes};

    // Return an error if the available space is insufficient.
    if (bytesAv < newSize)
    {
        const QString bytesAvStr{sizeToString(bytesAv)};
        const QString totalBytesStr{sizeToString(newSize)};

        const QString error{
            QString{"Cannot add new songs due to not enough space.\nOnly %1 available on disk: %2.\nTotal combined file size: %3"}
                .arg(bytesAvStr, storage.rootPath(), totalBytesStr)
        };

        return OperationResult::fail(error);
    }

    // Update the tracked total after the space check succeeds.
    m_TotalBytes = newSize;

    return OperationResult::succeed();
}
