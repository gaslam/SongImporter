#ifndef BYTESTRACKER_H
#define BYTESTRACKER_H

class SONGIMPORTERLIB_EXPORT BytesTracker
{
public:
	[[nodiscard]] OperationResult checkSpaceAvailable(const QList<QUrl>& files, const QString& destination);
private:
	qint64 m_TotalBytes;
};
#endif
