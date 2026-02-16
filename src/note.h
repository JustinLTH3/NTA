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
}

#endif //NTA_NOTE_H
