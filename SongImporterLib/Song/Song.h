#ifndef SONG_H
#define SONG_H

struct SONGIMPORTERLIB_EXPORT Song {
	Q_GADGET
	Q_PROPERTY(unsigned int year MEMBER year)
	Q_PROPERTY(unsigned int bpm MEMBER bpm)
	Q_PROPERTY(QString title MEMBER title)
	Q_PROPERTY(QString album MEMBER album)
	Q_PROPERTY(QString artists MEMBER artists)
	Q_PROPERTY(QString albumArtists MEMBER albumArtists)
	Q_PROPERTY(QString filename MEMBER filename)

public:

	unsigned int year;
	QString title;
	QString album;
	QString artists;
	QString albumArtists;
	QString filename;
    QString coverId;
    unsigned int bpm{0};

	bool operator==(const Song& other) const
	{
		return filename == other.filename;
	}

	bool operator==(const QString& filename) const
	{
		return this->filename == filename;
	}

	operator bool()
	{
		const bool bIsEmpty{ this->filename.isEmpty() };

		return !bIsEmpty;
	}
};

Q_DECLARE_METATYPE(Song)

#endif
