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
         * @param from The source id of the link.
         * @param to The target id of the link.
         * @return Success or not.
         */
        bool addLink(int64_t from, int64_t to);
        bool deleteNote(int64_t id);
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
