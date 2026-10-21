#pragma once
#include <QStandardPaths>
#include <QDir>
#include <QUrl>
#include <QuaZip.h>
#include <QCoreApplication>
/// I have multiple questions for this class
/// If I want to expose them in QML, I can't just put them inside of functions, I must use a class
/// At the same time, audio formats do not belong in in this class.
/// I will refactor this class when time possible.
/// Also, this class gives warnings.
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
        case SupportedAudioFormats::MP4:    return QStringLiteral("mp4");
        default:                            return QStringLiteral("unknown");
        }
    }

    /// Retreives the data file to be read by the dj software.
    /// \param software The specific software used for this application. This will be selected through a dropdown.
	[[nodiscard]] inline static QString softwareDataLocation(const SupportedSoftware& software)
	{
        QDir installDir{ QCoreApplication::applicationDirPath() + "/ExportDefaultFiles/" };

		switch (software)
		{

		case Rekordbox:
            installDir = installDir.filePath("./rekordbox.xml");
			break;
		}

		return installDir.absolutePath();
	}

    /// Checks if the audio format is supported for the specificly chosen software.
    /// \param software The specific software used for this application. This will be selected through a dropdown.
    /// \param audio The audio format to check.
	[[nodiscard]] inline static bool supportsAudioFormat(const SupportedSoftware& software, const SupportedAudioFormats& audio)
	{
		if (!m_SupportedFileFormats.contains(software))
		{
			return false;
		}

		return m_SupportedFileFormats[software].contains(audio);
	}

    /// Checks if the audio format is supported for the specificly chosen software.
    /// \param software The specific software used for this application. This will be selected through a dropdown.
    /// \param audio The audio format to check.
	[[nodiscard]] inline static bool supportsAudioFormats(const SupportedSoftware& software, const QString& audioFormat)
	{
		const SupportedAudioFormats audio{ stringToAudioFormat(audioFormat) };
		return supportsAudioFormat(software,audio);
	}

    /// Checks if the audio format is supported for the specificly chosen software.
    /// \param software The specific software used for this application. This will be selected through a dropdown.
    /// \param file The url of the selected file.
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

    /// When the user of the application selects a few files to be copied, this function checks all chosen file types based on validity.
    /// \param software The specific software used for this application. This will be selected through a dropdown.
    /// \param files The urls of the selected files.
    [[nodiscard]] inline static bool isFileListValid(const SupportedSoftware& software, const QList<QUrl>& files)
    {
        return std::all_of(files.begin(),files.end(),[software](const QUrl& file){
            const QFileInfo info{ file.toLocalFile()};

            if (!info.exists())
            {
                return false;
            }
            const SupportedAudioFormats audio{ stringToAudioFormat(info.suffix()) };
            return supportsAudioFormat(software, audio);
        });
    }

    /// When the user of the application selects a few files to be copied, this function checks all chosen file types based on validity.
    /// \param software The specific software used for this application. This will be selected through a dropdown.
    /// \param files The urls of the selected files.
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

    /// When the user of the application selects a few files to be copied, this function checks all chosen file types based on validity.
    /// \param software The specific software used for this application. This will be selected through a dropdown.
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

