#include "BytesTracker.h"
#include <QUrl>
#include <QFileInfo>
#include <QuaZip.h>
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
	return OperationResult::succeed();
}
