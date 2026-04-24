#include "FileUtils.h"

#include <QStandardPaths>

FileUtils::FileUtils(QObject *parent)
	: QObject(parent)
{}

//Rather can constantly calling QStandardPaths, I can just type this.
inline QString FileUtils::getUserMusicFolder()
{
	return QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
}

