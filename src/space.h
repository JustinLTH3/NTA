#pragma once
#include "QDir"
#include "SQLiteCpp/SQLiteCpp.h"
#include "QSharedPointer"

namespace NTA
{
    constexpr unsigned long applicationId = 0x4E5441;

    class Space
    {
    public:
        static Space* createStorage(QDir path, QString name);
        ~Space();
        QSharedPointer<SQLite::Database> getFile() { return file; }

    protected:
        explicit Space(const QSharedPointer<SQLite::Database>& in_file);

    protected:
        QSharedPointer<SQLite::Database> file;
    };
} // NTA
