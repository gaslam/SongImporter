#include "BytesTracker.h"
#include <QUrl>
#include <QFileInfo>
#include <QuaZip.h>
#include <QStorageInfo>
#include "FileUtils.h"

OperationResult BytesTracker::checkSpaceAvailable(const QList<QUrl>& files, const QString& destination)
{
        QMutexLocker locker(&m_Mutex);
	qint64 size{};
    foreach(const QUrl& file, files)
	{

		if (FileUtils::isNonEmptyZipFile(file))
		{
            QuaZip zip{ file.toString() };

            if (!zip.open(QuaZip::mdUnzip))
				continue;

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
			QFileInfo info{ file.toString() };
			size += info.size();
		}
	}

    const QStorageInfo storage {destination};

    const qint64 bytesAv{storage.bytesAvailable()};
    const qint64 newSize{size + m_TotalBytes};

    if(bytesAv < newSize)
    {
        QString bytesAvStr{sizeToString(bytesAv)};
        QString totalBytesStr{sizeToString(newSize)};

        const QString error{QString{"Cannot add new songs due to not enough space.\nOnly %1 available on disk: %2.\nTotal combined file size: %3"}
            .arg(bytesAvStr,storage.rootPath(),totalBytesStr)};

        return OperationResult::fail(error);
    }

    m_TotalBytes = newSize;

	return OperationResult::succeed();
}
