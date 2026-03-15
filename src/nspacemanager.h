#pragma once
#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include <SQLiteCpp/Statement.h>

#include "note.h"


namespace NTA
{
    class NWidget;
    struct Note;
    class Space;

    class NNoteManager : public QObject
    {
        Q_OBJECT

    public:
        static QPointer<NNoteManager> getInstance();
        static void destroyInstance();
        static bool createInstance(const QSharedPointer<Space>& in_space);
        QSharedPointer<Space> getSpace() const;

        /**
         * @param typeId The type of the note. Must be existed in the note_type table.
         * @param title The title of the note. There can be notes with the same title.
         * @return The created note. Nullptr if not successful
         */
        QSharedPointer<Note> createNote(int64_t typeId = 1, const QString& title = "New note");
        /**
         * @param id The id of the note.
         * @return The retrieved note with all content. Nullptr if note not exists.
         */
        QSharedPointer<Note> getNoteWithId(int64_t id);
        /**
         * @param param Text for searching notes that match param. Title and body are considered.
         * @return A sqlite statement for execution. Only contains id and is ordered by rank.
         */
        SQLite::Statement searchNotes(const QString& param, unsigned int columns = NoteColumn::id);

    protected:
        explicit NNoteManager(const QSharedPointer<Space>& in_space);
        QSharedPointer<Space> space;
        static QPointer<NNoteManager> instance;

        QMap<int64_t, QWeakPointer<Note>> cachedNotes;

    public:
        bool updateNote(int64_t id, Note updateData, NWidget* from, unsigned int columns = NoteColumn::body);
    signals:
        void noteUpdated(int64_t id, NWidget* from, unsigned int columns);
    };
}
