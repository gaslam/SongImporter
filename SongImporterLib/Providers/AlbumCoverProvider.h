#ifndef ALBUMCOVERPROVIDER_H
#define ALBUMCOVERPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QImage>
#include <QMutex>
#include "Song/Song.h"

namespace TagLib{
class FileRef;
}

namespace TagLib::ID3v2 {
class Tag;
}


/// This is the image importer. Gui applications when request it for generating albumCovers
/// To minimize too much memory on the stack, the images are kept in m_AlbumCovers.
/// If the image already exists, no extra memory is used.
class SONGIMPORTERLIB_EXPORT AlbumCoverProvider : public QQuickImageProvider
{
    Q_OBJECT
public:

    /// \param defaultId The default id that's used when no image is extracted or a problem occurs.
    /// \param defaultLogo The default logo that will show when no image is extracted or a problem occurs.
    explicit AlbumCoverProvider(QString& defaultId,QImage& defaultLogo);

    /// Qt's derived function to request an image.
    /// \param id The image id received throughout Qt's image provider url.
    /// \param size The size of the image that must get passed to render.
    /// \param requestedSize The requested size of the image that is scaled inside the function.

    /// Checks if the image provider already has a cover of the song.
    /// \param song A pointer to the song struct of the album cover.
    /// \return Returns true if the cover is already associated with an image. Otherwise, it returns false.
    bool hasCover(Song* song);

    /// Assigns the image to a song.
    /// \param song A pointer to the song struct of the album cover.
    /// \param song A file reference to the audio file of the album cover
    void setImage(Song* song,TagLib::FileRef& fileref);


private:
    /// A hashmap that stores album covers by it's id.
    QHash<QString,QImage> m_AlbumCovers;
    /// The default key set by the users gui application in case the image cannot be extracted.
    QString m_DefaultCoverKey;
    QMutex m_Locker;

    /// Extracts the album cover from an audio file
    /// \param fileref A reference to the TagLib file
    /// \param id The id of the album cover image
    /// \param song A pointer to the song struct of the album cover.
    void extractAlbumCoverArt(TagLib::FileRef& fileref, const QString& id, Song* song);

    /// Extracts the album cover from a tag
    /// \param tag A pointer pointing towards the ID3v2 tag of the file.
    /// \return The extracted album cover of the file's ID3v2 tag. Otherwise nothing returns.
};

#endif // ALBUMCOVERPROVIDER_H
