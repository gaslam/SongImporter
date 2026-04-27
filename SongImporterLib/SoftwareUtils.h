#pragma once
#include <QStandardPaths>
#include <QDir>

class SoftwareUtils  : public QObject
{
	Q_OBJECT
	enum SupportedAudioFormats {
		UNKNOWN,
		MP3,
		AIFF,
		WAV,
		FLAC,
		ALAC,
		AAC,
	};
	Q_ENUM(SupportedAudioFormats)

	enum SupportedSoftware {
		Rekordbox
	};
	Q_ENUM(SupportedSoftware)

public:
	SoftwareUtils(QObject *parent);
	~SoftwareUtils();

public slots:
	[[nodiscard]] static SupportedAudioFormats stringToAudioFormat(const QString& audioFormat);
	[[nodiscard]] inline static QString softwareDataLocation(const SupportedSoftware& software)
	{
		QDir installDir{ QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };

		switch (software)
		{

		case Rekordbox:
			installDir = installDir.filePath("../rekordbox/rekordbox/rekordbox.xml");
			break;
		}

		return installDir.absolutePath();
	}
};

