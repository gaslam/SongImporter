#ifndef SONG_H
#define SONG_H

struct Song {
	Q_GADGET

public:
	Song(const unsigned int& songYear,
		const QString& songTitle,
		const QString& songAlbum,
		const QString& songArtists,
		const QString& songAlbumArtists,
		const QString& songFile,
		const unsigned int songBpm):
		year{songYear},
		album{songAlbum},
		title{songTitle},
		artists{songArtists},
		albumArtists{songAlbumArtists},
		filename{songFile},
		bpm{songBpm}
	{

	}

	const unsigned int year;
	const QString title;
	const QString album;
	const QString artists;
	const QString albumArtists;
	const QString filename;
	const unsigned int bpm;
};

Q_DECLARE_METATYPE(Song)

#endif