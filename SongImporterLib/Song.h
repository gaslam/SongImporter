#ifndef SONG_H
#define SONG_H

struct Song {
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
	unsigned int bpm;
};

Q_DECLARE_METATYPE(Song)

#endif