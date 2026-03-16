#ifndef NTA_NOTE_H
#define NTA_NOTE_H
#include <qdatetime.h>
#include <qstring.h>

namespace NTA
{
    struct Note
    {
        int64_t id;
        QString title;
        QString body;
        int64_t typeId;
        QDateTime createdAt;
        QDateTime updatedAt;
    };

    enum NoteColumn
    {
        id = 1 << 0,
        title = 1 << 1,
        body = 1 << 2,
        typeId = 1 << 3,
        createdAt = 1 << 4,
        updatedAt = 1 << 5
    };

    struct NoteLink
    {
        int64_t source_id;
        int64_t target_id;
        QString description;
        QString alias;
    };
}

#endif //NTA_NOTE_H
