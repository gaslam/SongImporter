#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <QStandardPaths>
#include <QFile>
#include <QUrl>

class FileUtils : public QObject
{
	Q_OBJECT

public:
	FileUtils(QObject* parent);
public slots:
	//Rather can constantly calling QStandardPaths, I can just type this.
	[[nodiscard]] inline static QString getUserMusicFolder()
	{
		return QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
	}

	[[nodiscard]] inline static OperationResult isNonEmptyZipFile(const QUrl& url)
	{
		QFile f(url.toString());


		if (!f.exists())
		{
			const QString error{ url.toString() + " cannot be read. Set the permisions to read to continue." };
			return OperationResult::fail(error);
		}

		if (!f.open(QIODevice::ReadWrite))
		{
			const QString error{ f.fileName() + "cannot be read. Set the permisions to read to continue." };
			return OperationResult::fail(error);
		}

		const QByteArray sig{ f.read(4) };

		if (sig == "PK\x03\x04") //Normal ZIP
		{
			return OperationResult::succeed();
		}
		else if (sig == "PK\x05\x06") //Empty ZIP
		{
			const QString error{ "ZIP archive is empty." };
			return OperationResult::fail(error);
		}

		const QString error{ "File is not a valid ZIP archive." };
		return OperationResult::fail(error);
	}
};
#endif // !FILEUTILS_H

