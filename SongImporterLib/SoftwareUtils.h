#pragma once

#include <QObject>
#include <QHash>

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
		Rekordbox,
	};
	Q_ENUM(SupportedSoftware)

	Q_PROPERTY(SupportedSoftware supportedSoftware READ supportedSoftware WRITE supportedSoftware NOTIFY supportedSoftwareChanged)

public:
	SoftwareUtils(QObject *parent);
	~SoftwareUtils();

public slots:
	[[nodiscard]] SupportedAudioFormats stringToAudioFormat(const QString& audioFormat);
	[[nodiscard]] SupportedSoftware supportedSoftware() const;
	void supportedSoftware(const SupportedSoftware& software);

	[[nodiscard]] QList<SupportedAudioFormats> supportedAudioFormats() const;

signals:
	void supportedSoftwareChanged(const SupportedSoftware& software);

private:
	SupportedSoftware m_SupportedSoftware{ Rekordbox }; // default

	QHash<SupportedSoftware, QList<SupportedAudioFormats>> m_SupportedAudioFormats{ {
			Rekordbox, {MP3,AIFF}
	} };
};

