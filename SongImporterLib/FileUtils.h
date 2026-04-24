#pragma once

#include <QObject>

class FileUtils  : public QObject
{
	Q_OBJECT

public:
	FileUtils(QObject *parent);

	[[nodiscard]] inline static QString getUserMusicFolder();
};

