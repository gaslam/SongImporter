#pragma once

#include <QObject>
#include <QStandardPaths>

class FileUtils  : public QObject
{
	Q_OBJECT

public:
	FileUtils(QObject *parent);
public slots:
	//Rather can constantly calling QStandardPaths, I can just type this.
	[[nodiscard]] inline static QString getUserMusicFolder()
	{
		return QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
	}
};

