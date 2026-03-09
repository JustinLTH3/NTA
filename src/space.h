#pragma once
#include "note.h"
#include "QDir"
#include "SQLiteCpp/SQLiteCpp.h"
#include "QSharedPointer"

namespace NTA
{
    struct Note;
    constexpr unsigned long applicationId = 0x4E5441;

    class Space
    {
    public:
        /**
         * @param path The path to create space at.
         * @param name The filename of the space.
         * @return Space Ptr created if successful, nullptr if not.
         */
        static Space* createSpace(const QDir& path, const QString& name);
        /**
         * @param path The path of the space file, which should contain the filename.
         * @return Space ptr created if successful, nullptr if not.
         */
        static Space* openExistingSpace(const QString& path);
        ~Space();
        /**
         * @return The database connection.
         */
        QSharedPointer<SQLite::Database> getFile() { return file; }

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
        // SQLite::Statement searchNotes(const QString& param);

        SQLite::Statement searchNotes(const QString& param, unsigned int columns = NoteColumn::id);
        /**
         * @param from The source id of the link.
         * @param to The target id of the link.
         * @return Success or not.
         */
        bool addLink(int64_t from, int64_t to);

    protected:
        /**
         * It is protected for preventing space initialization other than the static createSpace/openExistingSpace function.
         * @param in_file the database connection.
         */
        explicit Space(const QSharedPointer<SQLite::Database>& in_file);

    protected:
        QSharedPointer<SQLite::Database> file;
    };
} // NTA
