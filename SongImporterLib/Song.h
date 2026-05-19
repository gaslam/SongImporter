#ifndef SONG_H
#define SONG_H

struct Song {
	Q_GADGET

	const unsigned int year;
	const QString title;
	const QString album;
	const QUrl albumCover;
	const QString songArtists;
	const QString albumArtists;
	const QString filename;
	const unsigned int bpm;
};

Q_DECLARE_METATYPE(Song)

#endif