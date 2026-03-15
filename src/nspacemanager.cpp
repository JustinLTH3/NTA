//
// Created by JustinLai on 8/3/2026.
//

#include "nspacemanager.h"

#include <QPointer>
#include <spdlog/spdlog.h>

#include "space.h"

namespace NTA
{
    QPointer<NNoteManager> NNoteManager::instance = nullptr;

    bool NNoteManager::updateNote(int64_t id, Note updateData, NWidget* from, unsigned int columns)
    {
        Q_ASSERT(columns != 0);
        auto note = getNoteWithId(id);
        if (!note)return false;

        if ((columns & (title | body)) == 0)return false;
        bool doUpdate = false;
        QString query = R"(UPDATE notes SET )";
        if (columns & title && note->title != updateData.title)
        {
            note->title = updateData.title;
            query.append(R"(title = :TIT, )");
            doUpdate = true;
        } else if (columns & title && note->title == updateData.title)columns ^= title;
        if (columns & body && note->body != updateData.body)
        {
            note->body = updateData.body;
            query.append(R"(body = :BOD, )");
            doUpdate = true;
        } else if (columns & body && note->body == updateData.body)columns ^= body;
        if (!doUpdate)return false;

        query.append("updated_at = :UPD");
        query.append(" WHERE id = :ID;");
        SQLite::Statement statement(*space->getFile(), query.toStdString());
        statement.bind(":ID", id);
        statement.bind(":UPD", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString());
        if (columns & title)statement.bind(":TIT", updateData.title.toStdString());
        if (columns & body)statement.bind(":BOD", updateData.body.toStdString());
        auto r = statement.exec();
        if (r)Q_EMIT noteUpdated(id, from, columns);
        return r;
    }

    QPointer<NNoteManager> NNoteManager::getInstance()
    {
        return instance;
    }

    void NNoteManager::destroyInstance()
    {
        if (instance)
        {
            instance->space.reset(nullptr);
            instance->deleteLater();
        }
        instance = nullptr;
    }

    bool NNoteManager::createInstance(const QSharedPointer<Space>& in_space)
    {
        if (!instance && in_space)
        {
            instance = new NNoteManager(in_space);
            return true;
        }
        return false;
    }

    QSharedPointer<Space> NNoteManager::getSpace() const
    {
        return space;
    }

    QSharedPointer<Note> NNoteManager::createNote(int64_t typeId, const QString& title)
    {
        auto file = space->getFile();
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
        cachedNotes[note->id] = note;
        return note;
    }

    QSharedPointer<Note> NNoteManager::getNoteWithId(int64_t id)
    {
        if (cachedNotes.contains(id) && cachedNotes[id])return cachedNotes[id];
        if (id < 0)return nullptr;
        auto file = space->getFile();
        SQLite::Statement statement(*file, "SELECT * FROM notes WHERE id = ?");
        statement.bind(1, id);
        if (statement.executeStep())
        {
            auto note = QSharedPointer<Note>(new Note{
                .id = id, .title = QString::fromStdString(statement.getColumn("title").getString()),
                .body = QString::fromStdString(statement.getColumn("body").getString()),
                .typeId = statement.getColumn("typeId").getInt64(),
                .createdAt = QDateTime::fromString(
                    QString::fromStdString(statement.getColumn("created_at").getString()),
                    "yyyy-MM-dd hh:mm:ss"),
                .updatedAt = QDateTime::fromString(
                    QString::fromStdString(statement.getColumn("updated_at").getString()),
                    "yyyy-MM-dd hh:mm:ss")
            });
            cachedNotes[id] = note;
            return note;
        }
        return nullptr;
    }

    SQLite::Statement NNoteManager::searchNotes(const QString& param, unsigned int columns)
    {
        Q_ASSERT(columns != 0);
        QString query = R"(SELECT )";
        if (columns & NoteColumn::id)query.append("rowid as id, ");
        if (columns & NoteColumn::title)query.append("title, ");
        if (columns & NoteColumn::body)query.append("body, ");
        if (columns & NoteColumn::typeId)query.append("typeId, ");
        if (columns & NoteColumn::createdAt)query.append("created_at, ");
        if (columns & NoteColumn::updatedAt)query.append("updated_at, ");
        query.removeAt(query.length() - 2);
        auto file = space->getFile();
        if (param.isEmpty())
        {
            query.append(" FROM notes_fts ORDER BY rank");
            return {*file, query.toStdString()};
        }
        query.append(R"(FROM notes_fts WHERE notes_fts MATCH '")");
        query.append(param);
        query.append(R"("' ORDER BY rank)");
        return {*file, query.toStdString()};
    }

    NNoteManager::NNoteManager(const QSharedPointer<Space>& in_space) : QObject(nullptr), space(in_space)
    {
    }
}
