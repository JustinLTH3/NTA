#include "space.h"

#include <spdlog/spdlog.h>
#include <sqlite3.h>

#include "note.h"
#include "SQLiteCpp/SQLiteCpp.h"
extern "C" int sqlite3_ftsicu_init(sqlite3* db, char** pzErrMsg, const sqlite3_api_routines* pApi);

namespace NTA
{
    Space* Space::createSpace(const QDir& path, const QString& name)
    {
        if (!path.exists())
        {
            spdlog::error("path {} not exist", path.filesystemAbsolutePath().string());
            return nullptr;
        }
        if (path.exists(name))
        {
            spdlog::error("file {} already exist", name.toStdString());
            return nullptr;
        }
        const auto filePath = path.absoluteFilePath(name);
        try
        {
            auto file = QSharedPointer<SQLite::Database>(
                new SQLite::Database(filePath.toStdString(), SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE));
            sqlite3_ftsicu_init(file->getHandle(), nullptr, nullptr);
            file->exec("PRAGMA application_id = " + std::to_string(applicationId) + " ;");
            file->exec(""
                "PRAGMA foreign_keys = ON;"
                "CREATE TABLE settings"
                "("
                "    key   TEXT PRIMARY KEY,"
                "    value TEXT"
                ");"
            );
            file->exec(" CREATE TABLE note_type"
                " ("
                "     id   INTEGER PRIMARY KEY,"
                "     name TEXT UNIQUE,"
                "     typeDef TEXT"
                " );"
                " "
                " INSERT INTO note_type (id, name, typeDef)"
                " VALUES (NULL, 'default', NULL);"
                " INSERT INTO note_type (id, name, typeDef)"
                " VALUES (NULL, 'board', NULL);"
            );
            file->exec(" CREATE TABLE notes"
                " ("
                "     id         INTEGER PRIMARY KEY,"
                "     title      TEXT NOT NULL,"
                "     body       TEXT,"
                "     typeId     INTEGER NOT NULL DEFAULT 1,"
                "     created_at TEXT          DEFAULT CURRENT_TIMESTAMP,"
                "     updated_at TEXT          DEFAULT CURRENT_TIMESTAMP,"
                "     FOREIGN KEY (typeId) REFERENCES note_type (id)"
                "         ON DELETE SET DEFAULT"
                "         ON UPDATE CASCADE"
                " );"
            );

            file->exec(" CREATE VIRTUAL TABLE notes_fts USING fts5"
                " ("
                "     title,"
                "     body,"
                "     content = 'notes',"
                "     content_rowid = id,"
                "     tokenize = 'icu'"
                " );"
            );
            file->exec(" CREATE TABLE note_links"
                " ("
                "     id        INTEGER PRIMARY KEY,"
                "     source_id INTEGER NOT NULL,"
                "     target_id INTEGER NOT NULL,"
                "     description TEXT,"
                "     FOREIGN KEY (source_id) REFERENCES notes (id)"
                "         ON DELETE CASCADE"
                "         ON UPDATE CASCADE,"
                "     FOREIGN KEY (target_id) REFERENCES notes (id)"
                "         ON DELETE CASCADE"
                "         ON UPDATE CASCADE"
                " );"
            );
            file->exec(" CREATE TABLE boards"
                " ("
                "     id   INTEGER NOT NULL,"
                "     note INTEGER NOT NULL CHECK (note != id),"
                "     x1    REAL, y1 REAL, x2 REAL, y2 REAL,"
                "     PRIMARY KEY (id, note),"
                "     FOREIGN KEY (id) REFERENCES notes (id)"
                "         ON DELETE CASCADE"
                "         ON UPDATE CASCADE,"
                "     FOREIGN KEY (note) REFERENCES notes (id)"
                "         ON DELETE CASCADE"
                "         ON UPDATE CASCADE"
                " );"
            );

            file->exec(" CREATE TRIGGER validate_board_note"
                "     BEFORE INSERT"
                "     ON boards"
                "     FOR EACH ROW"
                " BEGIN"
                "     SELECT CASE"
                "                WHEN (SELECT type FROM notes WHERE id = NEW.id) != 'board'"
                "                    THEN RAISE(ABORT, 'Board must be attached to a board note')"
                "                END;"
                " END;"
            );
            file->exec(
                " CREATE TRIGGER update_note_fts"
                "     AFTER UPDATE"
                "     ON notes"
                "     FOR EACH ROW"
                " BEGIN"
                "     INSERT INTO notes_fts(notes_fts, ROWID, title, body) VALUES ('delete', OLD.id, OLD.title, OLD.body);"
                "     INSERT INTO notes_fts (rowid, title, body)"
                "     VALUES (NEW.id, NEW.title, NEW.body);"
                " end;"
                " "
                " CREATE TRIGGER insert_note_fts"
                "     AFTER INSERT"
                "     ON notes"
                "     FOR EACH ROW"
                " BEGIN"
                "     INSERT INTO notes_fts (rowid, title, body)"
                "     VALUES (NEW.id, NEW.title, NEW.body);"
                " END;"
                " "
                " CREATE TRIGGER delete_note_fts"
                "     AFTER DELETE"
                "     ON notes"
                " BEGIN"
                "     INSERT INTO notes_fts(notes_fts, ROWID, title, body) VALUES ('delete', OLD.id, OLD.title, OLD.body);"
                " END;"
                ""
            );

            return new Space(file);
        } catch (std::exception& e)
        {
            spdlog::error("Failed to create storage {}", e.what());
        }
        return nullptr;
    }

    Space* Space::openExistingSpace(const QString& path)
    {
        if (path.isNull() || path.isEmpty())return nullptr;
        try
        {
            const auto file = QSharedPointer<SQLite::Database>(
                new SQLite::Database(path.toStdString(), SQLite::OPEN_READWRITE));
            auto header = file->getHeaderInfo();
            if (header.applicationId != applicationId)
            {
                spdlog::error("file {} is not a NTA, appid: {}", path.toStdString(), header.applicationId);
                return nullptr;
            }
            if (!file->tableExists("notes"))
            {
                spdlog::error("table notes not exist");
                return nullptr;
            }
            if (!file->tableExists("notes_fts"))
            {
                spdlog::error("table notes_fts not exist");
                return nullptr;
            }
            if (!file->tableExists("note_links"))
            {
                spdlog::error("table note_links not exist");
                return nullptr;
            }
            if (!file->tableExists("boards"))
            {
                spdlog::error("table boards not exist");
                return nullptr;
            }
            if (!file->tableExists("note_type"))
            {
                spdlog::error("table note_type not exist");
                return nullptr;
            }
            if (!file->tableExists("settings"))
            {
                spdlog::error("table settings not exist");
                return nullptr;
            }

            SQLite::Statement integrityCheck(*file, "PRAGMA integrity_check;");
            integrityCheck.executeStep();
            if (integrityCheck.getColumn(0).getString() != "ok")
            {
                spdlog::error("database is corrupted");
                return nullptr;
            }

            sqlite3_ftsicu_init(file->getHandle(), nullptr, nullptr);
            file->exec("PRAGMA foreign_keys = ON;");
            return new Space(file);
        } catch (std::exception& e)
        {
            spdlog::error("Failed to open storage {}", e.what());
        }
        return nullptr;
    }

    Space::~Space()
    {
    }

    QSharedPointer<Note> Space::createNote(const int64_t typeId, const QString& title)
    {
        //Check typeId exist in table
        {
            SQLite::Statement statement(*file, "SELECT * FROM note_type WHERE id = ?");
            statement.bind(1, typeId);
            if (!statement.executeStep())
            {
                spdlog::error("typeId {} not exist", typeId);
                return nullptr;
            }
        }
        SQLite::Statement statement(*file, "INSERT INTO notes (id, title, body, typeId) VALUES (null, ?, null, ?)");

        statement.bind(1, title.toStdString());
        statement.bind(2, typeId);
        statement.exec();
        SQLite::Statement r(*file, "SELECT * FROM notes WHERE id = last_insert_rowid()");
        r.executeStep();
        auto note = QSharedPointer<Note>(new Note{
            .id = r.getColumn(0).getInt64(), .title = title,
            .body = QString::fromStdString(r.getColumn(2).getString()), .typeId = typeId,
            .createdAt = QDateTime::fromString(QString::fromStdString(r.getColumn(4).getString()),
                                               "yyyy-MM-dd hh:mm:ss"),
            .updatedAt = QDateTime::fromString(QString::fromStdString(r.getColumn(5).getString()),
                                               "yyyy-MM-dd hh:mm:ss")
        });
        return note;
    }

    Space::Space(const QSharedPointer<SQLite::Database>& in_file) : file(in_file)
    {
    }
} // NTA
