#include "BytesTracker.h"
#include <QUrl>
#include <QFileInfo>
#include <QuaZip.h>
#include <QStorageInfo>
#include "FileUtils.h"

OperationResult BytesTracker::checkSpaceAvailable(const QList<QUrl>& files, const QString& destination)
{
	qint64 size{};
	foreach(const auto& file, files)
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

    QStorageInfo storage = QStorageInfo{destination};

    auto bytesAv{storage.bytesAvailable()};
    qint64 newSize{size + m_TotalBytes};

    if(bytesAv < newSize)
    {

        const QString error{QString{"test"}};
        return OperationResult::fail(error);
    }

    m_TotalBytes = newSize;


	return OperationResult::succeed();
}
