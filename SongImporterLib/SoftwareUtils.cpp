#include "SoftwareUtils.h"
#include <QMetaEnum>

SoftwareUtils::SoftwareUtils(QObject *parent)
	: QObject(parent)
{}

SoftwareUtils::~SoftwareUtils()
{}

SoftwareUtils::SupportedAudioFormats SoftwareUtils::stringToAudioFormat(const QString & audioFormat)
{
	QString audioFormatUpper{ audioFormat.toUpper()};
	audioFormatUpper.replace(".", "");

	QMetaEnum metaEnum = QMetaEnum::fromType<SupportedAudioFormats>();
	int value = metaEnum.keyToValue(audioFormatUpper.toUtf8().constData());

	return value == -1
		? SupportedAudioFormats::UNKNOWN
		: static_cast<SupportedAudioFormats>(value);
}

