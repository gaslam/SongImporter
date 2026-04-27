#pragma once

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

public:
	SoftwareUtils(QObject *parent);
	~SoftwareUtils();

public slots:
	[[nodiscard]] static SupportedAudioFormats stringToAudioFormat(const QString& audioFormat);
};

