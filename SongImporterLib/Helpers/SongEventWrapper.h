#ifndef SONGEVENTWRAPPER_H
#define SONGEVENTWRAPPER_H
#include "../Song.h"
#include <fileref.h>

struct SONGIMPORTERLIB_EXPORT SongEventWrapper{
    Q_GADGET

public:

    SongEventWrapper(Song& chosenSong, TagLib::FileRef& ref):
        song{chosenSong},
        fileRef{ref}
    {

    }

    Song song;
    TagLib::FileRef fileRef;
};

Q_DECLARE_METATYPE(SongEventWrapper)

#endif // SONGEVENTWRAPPER_H
