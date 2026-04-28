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

	using SoftwareAudioFormats = QHash<SupportedSoftware, QList<SupportedAudioFormats>>;

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

	[[nodiscard]] inline static bool supportsAudioFormat(const SupportedSoftware& software, const SupportedAudioFormats& audio)
	{
		if (!m_SupportedFileFormats.contains(software))
		{
			return false;
		}

		return m_SupportedFileFormats[software].contains(audio);
	}

	[[nodiscard]] inline static bool supportsAudioFormats(const SupportedSoftware& software, const QString& audioFormat)
	{
		const SupportedAudioFormats audio{ stringToAudioFormat(audioFormat) };
		return supportsAudioFormat(software,audio);
	}

private:
	inline static SoftwareAudioFormats m_SupportedFileFormats = {
		{ SoftwareUtils::Rekordbox, { SoftwareUtils::AIFF, SoftwareUtils::MP3 } }
	};
};

