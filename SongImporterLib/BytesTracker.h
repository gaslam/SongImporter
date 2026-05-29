#ifndef BYTESTRACKER_H
#define BYTESTRACKER_H

#include <QMutex>

class SONGIMPORTERLIB_EXPORT BytesTracker
{
public:
	[[nodiscard]] OperationResult checkSpaceAvailable(const QList<QUrl>& files, const QString& destination);
private:
    [[nodiscard]]static inline QString sizeToString(const qint64& bytes)
    {
        constexpr double kb { 1024.0};
        constexpr double mb { kb * kb};
        constexpr double gb { mb * kb};

        constexpr char floatFormat{'f'};
        constexpr int precision{2};

        if (bytes >= gb)
            return QString::number(bytes / gb, floatFormat, precision) + " GB";
        else if (bytes >= mb)
            return QString::number(bytes / mb, floatFormat, precision) + " MB";

        return QString::number(bytes / kb, floatFormat, precision) + " KB";
    };

	qint64 m_TotalBytes;

    QMutex m_Mutex;
};
#endif
