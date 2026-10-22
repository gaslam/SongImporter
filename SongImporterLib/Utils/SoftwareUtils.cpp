#include "SoftwareUtils.h"
#include <QMetaEnum>

SoftwareUtils::SupportedAudioFormats SoftwareUtils::stringToAudioFormat(const QString & audioFormat)
{
    /// Prevent any dots from appearing in the format.
	QString audioFormatUpper{ audioFormat.toUpper()};
	audioFormatUpper.replace(".", "");

    /// Get metaEnum from type of SupportedAudioFormats
    /// Turn the string into it's representing value.
    /// I just found that converting to UTF-8 seems to get the right variable type to convert to audio.
	QMetaEnum metaEnum = QMetaEnum::fromType<SupportedAudioFormats>();
	int value = metaEnum.keyToValue(audioFormatUpper.toUtf8().constData());

	return value == -1
		? SupportedAudioFormats::UNKNOWN
		: static_cast<SupportedAudioFormats>(value);
}

