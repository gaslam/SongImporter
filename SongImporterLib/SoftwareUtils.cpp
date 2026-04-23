#include "SoftwareUtils.h"
#include <QMetaEnum>

SoftwareUtils::SoftwareUtils(QObject *parent)
	: QObject(parent)
{}

SoftwareUtils::~SoftwareUtils()
{}

SoftwareUtils::SupportedSoftware SoftwareUtils::supportedSoftware() const
{
	return m_SupportedSoftware;
}

void SoftwareUtils::supportedSoftware(const SupportedSoftware& software)
{
	if (m_SupportedSoftware == software)
	{
		return;
	}

	m_SupportedSoftware = software;
	emit supportedSoftwareChanged(software);
}

QList<SoftwareUtils::SupportedAudioFormats> SoftwareUtils::supportedAudioFormats() const
{
	return m_SupportedAudioFormats[m_SupportedSoftware];
}

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

