#pragma once

#include <QObject>

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
};

