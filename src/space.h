#pragma once
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
        QSharedPointer<SQLite::Database> getFile() { return file; }

        QSharedPointer<Note> createNote(int64_t typeId = 1, const QString& title = "New note");

    protected:
        explicit Space(const QSharedPointer<SQLite::Database>& in_file);

    protected:
        QSharedPointer<SQLite::Database> file;
    };
} // NTA
