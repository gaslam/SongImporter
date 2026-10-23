#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <QStandardPaths>
#include <QFile>
#include <QUrl>
#include <quazip.h>

class SONGIMPORTERLIB_EXPORT FileUtils : public QObject
{
	Q_OBJECT

public:
	FileUtils(QObject* parent);
public slots:
    /// Rather than constantly calling QStandardPaths, I can just type this.
    ///\returns The computers local music folder
	[[nodiscard]] inline static QString getUserMusicFolder()
	{
		return QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
	}

    /// A helper function to turn the url into a local file in qml.
    ///\returns The requested url.
	[[nodiscard]] inline static QString toLocalFile(const QUrl& url)
	{
		return url.toLocalFile();
	}

    /// Checks whether the specified file is a non-empty ZIP archive.
    /// \param url The URL of the ZIP archive to check.
    /// \returns A successful result if the ZIP signature indicates a non-empty archive;
    /// otherwise, a failing result describing the error.
    [[nodiscard]] inline static OperationResult isNonEmptyZipFile(const QUrl& url)
    {

        ///Check if the file can be read or opened.
        QFile f(url.toString());

        if (!f.exists())
        {
            const QString error{url.toString() + " cannot be read. Set the permissions to read to continue."};
            return OperationResult::fail(error);
        }

        if (!f.open(QIODevice::ReadWrite))
        {
            const QString error{f.fileName() + " cannot be read. Set the permissions to read to continue."};
            return OperationResult::fail(error);
        }

        /// Read the first four bytes to identify the ZIP file signature.
        const QByteArray sig{f.read(4)};

        /// Check for the signature of a ZIP archive containing file entries.
        if (sig == "PK\x03\x04")
        {
            return OperationResult::succeed();
        }
        /// Check for the end-of-central-directory signature of an empty ZIP archive.
        else if (sig == "PK\x05\x06")
        {
            const QString error{"ZIP archive is empty."};
            return OperationResult::fail(error);
        }

        /// Return an error if the file does not match either recognized signature.

        const QString error{"File is not a valid ZIP archive."};
        return OperationResult::fail(error);
    }
};
#endif // !FILEUTILS_H

