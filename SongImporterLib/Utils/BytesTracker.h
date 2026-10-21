#ifndef BYTESTRACKER_H
#define BYTESTRACKER_H

#include <QMutex>

/// Checks available disk space for files being processed.
class SONGIMPORTERLIB_EXPORT BytesTracker
{
public:
    /// Checks whether sufficient disk space is available for the specified files.
    /// \param files The files to be processed.
    /// \param destination The destination directory.
    /// \return The result of the disk space check.
    [[nodiscard]] OperationResult checkSpaceAvailable(const QList<QUrl>& files, const QString& destination);

private:
    /// Converts a byte count to a human-readable size in KB, MB or GB.
    /// The result is formatted to two decimal places using a base of 1024.
    /// \param bytes The size in bytes.
    /// \return The formatted size as a string.
    [[nodiscard]] static inline QString sizeToString(const qint64& bytes)
    {
        constexpr double kb{1024.0};
        constexpr double mb{kb * kb};
        constexpr double gb{mb * kb};

        constexpr char floatFormat{'f'};
        constexpr int precision{2};

        if (bytes >= gb)
            return QString::number(bytes / gb, floatFormat, precision) + " GB";
        else if (bytes >= mb)
            return QString::number(bytes / mb, floatFormat, precision) + " MB";

        return QString::number(bytes / kb, floatFormat, precision) + " KB";
    }

    /// Stores the total size of the files in bytes.
    qint64 m_TotalBytes{};

    /// Protects shared data from concurrent access.
    QMutex m_Mutex;
};

#endif
