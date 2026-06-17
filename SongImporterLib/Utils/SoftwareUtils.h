#pragma once
#include <QStandardPaths>
#include <QDir>
#include <QUrl>
#include <QuaZip.h>

class SONGIMPORTERLIB_EXPORT SoftwareUtils  : public QObject
{
	Q_OBJECT

public:
    enum SupportedAudioFormats {
        UNKNOWN,
        MP3,
        AIFF,
        WAV,
        FLAC,
        ALAC,
        AAC,
        ZIP,
    };
    Q_ENUM(SupportedAudioFormats)

    enum SupportedSoftware {
        Rekordbox
    };
    Q_ENUM(SupportedSoftware)

    using SoftwareAudioFormats = QHash<SupportedSoftware, QList<SupportedAudioFormats>>;
	SoftwareUtils(QObject *parent);
	~SoftwareUtils();

public slots:
	[[nodiscard]] static SupportedAudioFormats stringToAudioFormat(const QString& audioFormat);

    [[nodiscard]] static QString supportedAudioFormatToString(SupportedAudioFormats f) {
        switch (f) {
        case SupportedAudioFormats::MP3:     return QStringLiteral("mp3");
        case SupportedAudioFormats::AIFF:     return QStringLiteral("aiff");
        case SupportedAudioFormats::WAV:     return QStringLiteral("wav");
        case SupportedAudioFormats::FLAC:    return QStringLiteral("flac");
        case SupportedAudioFormats::ALAC:    return QStringLiteral("alac");
        case SupportedAudioFormats::AAC:     return QStringLiteral("aac");
        case SupportedAudioFormats::ZIP:     return QStringLiteral("zip");
        default:                            return QStringLiteral("unknown");
        }
    }
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

	[[nodiscard]] inline static bool test()
	{
		QuaZip zip{ "test.zip" };

		return true;
	}

	[[nodiscard]] inline static bool supportsAudioFormats(const SupportedSoftware& software, const QString& audioFormat)
	{
		const SupportedAudioFormats audio{ stringToAudioFormat(audioFormat) };
		return supportsAudioFormat(software,audio);
	}

	[[nodiscard]] inline static bool supportsAudioFormatFromUrl(const SupportedSoftware& software, const QUrl& file)
	{
		const QFileInfo info{ file.toLocalFile()};

		if (!info.exists())
		{
			return false;
		}
		const SupportedAudioFormats audio{ stringToAudioFormat(info.suffix()) };
        return supportsAudioFormat(software, audio);
	}

    [[nodiscard]] inline static QList<QString> supportedAudioFormatsString(const SupportedSoftware& software)
    {
        QList<SupportedAudioFormats> formats{supportedAudioFormats(software)};
        QList<QString> result{};
        result.reserve(formats.size());

        foreach(auto& format, formats)
        {
            result << supportedAudioFormatToString(format);
        }

        return result;
    }

    [[nodiscard]] inline static QList<SupportedAudioFormats> supportedAudioFormats(const SupportedSoftware& software)
    {
        if(m_SupportedFileFormats.contains(software))
        {
            return m_SupportedFileFormats[software];
        }

        return {};
    }

private:
	inline static SoftwareAudioFormats m_SupportedFileFormats = {
        { SoftwareUtils::Rekordbox, { SoftwareUtils::AIFF,SoftwareUtils::FLAC, SoftwareUtils::MP3, SoftwareUtils::ZIP } }
	};
};

