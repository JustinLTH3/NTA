//
// Created by JustinLai on 15/3/2026.
//

#include "nlinkmanager.h"

#include <SQLiteCpp/Statement.h>
#include "space.h"
#include <QPointer>
#include <qregularexpression.h>
#include <spdlog/spdlog.h>
#include "QStringBuilder"

namespace NTA
{
    QPointer<NLinkManager> NLinkManager::instance = nullptr;

    QPointer<NLinkManager> NLinkManager::getInstance()
    {
        return instance;
    }

    void NLinkManager::destroyInstance()
    {
        if (instance)
        {
            instance->space.reset(nullptr);
            instance->deleteLater();
        }
        instance = nullptr;
    }

    bool NLinkManager::createInstance(const QSharedPointer<Space>& in_space)
    {
        if (!instance && in_space)
        {
            instance = new NLinkManager(in_space);
            return true;
        }
        return false;
    }

    NLinkManager::NLinkManager(const QSharedPointer<Space>& in_space) : QObject(nullptr), space(in_space)
    {
    }

    bool NLinkManager::addLink(int64_t from, int64_t to)
    {
        if (from == to)return false;
        SQLite::Statement check(*space->getFile(), "SELECT id FROM notes WHERE id = ? OR id = ?");
        check.bind(1, from);
        check.bind(2, to);
        if (!check.executeStep())return false;
        if (!check.executeStep())return false;
        SQLite::Statement statement(*space->getFile(),
                                    "INSERT OR IGNORE INTO note_links (source_id, target_id, alias, description) VALUES (?1, ?2, (SELECT title FROM notes WHERE id = ?2), null)");
        statement.bind(1, from);
        statement.bind(2, to);
        return statement.exec();
    }

    bool NLinkManager::removeLink(int64_t from, int64_t to)
    {
        SQLite::Statement statement(*space->getFile(), "DELETE FROM note_links WHERE source_id = ? AND target_id = ?");
        statement.bind(1, from);
        statement.bind(2, to);
        return statement.exec();
    }

    bool NLinkManager::editLink(int64_t from, int64_t to, QString description, QString alias)
    {
        SQLite::Statement statement(*space->getFile(),
                                    "UPDATE note_links SET description = ?, alias = ? WHERE source_id = ? AND target_id = ?");
        statement.bind(1, description.toStdString());
        statement.bind(2, alias.toStdString());
        statement.bind(3, from);
        statement.bind(4, to);
        return statement.exec();
    }

    SQLite::Statement NLinkManager::getLinks(int64_t from)
    {
        SQLite::Statement statement(*space->getFile(), "SELECT * FROM note_links WHERE source_id = ?");
        statement.bind(1, from);
        return statement;
    }

    SQLite::Statement NLinkManager::getLink(int64_t from, int64_t to, unsigned int columns)
    {
        Q_ASSERT(columns != 0);
        QString query = R"(SELECT )";
        if (columns & NoteLinkColumns::source_id)query.append("source_id_id, ");
        if (columns & NoteLinkColumns::target_id)query.append("target_id_id, ");
        if (columns & NoteLinkColumns::description)query.append("description, ");
        if (columns & NoteLinkColumns::alias)query.append("alias, ");
        query.removeAt(query.length() - 2);
        query.append(" FROM note_links WHERE source_id = ? AND target_id = ?");
        SQLite::Statement statement(*space->getFile(), query.toStdString());
        statement.bind(1, from);
        statement.bind(2, to);
        return statement;
    }

    SQLite::Statement NLinkManager::getBackLinks(int64_t to)
    {
        SQLite::Statement statement(*space->getFile(), "SELECT * FROM note_links WHERE target_id = ?");
        statement.bind(1, to);
        return statement;
    }

    SQLite::Statement NLinkManager::searchLinks(QString param)
    {
        param.replace(QRegularExpression("([%_])"), "\\\\1");
        param.prepend("%");
        param.append("%");
        SQLite::Statement statement(*space->getFile(),
                                    "SELECT * FROM note_links WHERE rowid IN (SELECT rowid FROM note_links_fts WHERE description LIKE ? ESCAPE '\\' OR alias LIKE ? ESCAPE '\\');");
        statement.bind(1, param.toStdString());
        statement.bind(2, param.toStdString());
        return statement;
    }

    SQLite::Statement NLinkManager::searchLinksOfSourceWithNoteTitle(QString param, int64_t sourceId)
    {
        param.replace(QRegularExpression("([%_])"), "\\\\1");
        param.prepend("%");
        param.append("%");
        SQLite::Statement statement(*space->getFile(),
                                    "SELECT note_links.*, notes.title FROM note_links LEFT JOIN notes ON notes.id = note_links.target_id WHERE source_id = ? AND note_links.rowid IN (SELECT rowid FROM note_links_fts WHERE description LIKE ? ESCAPE '\\' OR alias LIKE ? ESCAPE '\\');");
        statement.bind(1, sourceId);
        statement.bind(2, param.toStdString());
        statement.bind(3, param.toStdString());
        return statement;
    }

    SQLite::Statement NLinkManager::searchNotesExcludeLinked(QString param, int64_t sourceId, unsigned int columns,
                                                             bool invert)
    {
        Q_ASSERT(columns != 0);
        QString query = R"(SELECT )";
        if (columns & NoteColumn::id)query.append("id, ");
        if (columns & NoteColumn::title)query.append("title, ");
        if (columns & NoteColumn::body)query.append("body, ");
        if (columns & NoteColumn::typeId)query.append("typeId, ");
        if (columns & NoteColumn::createdAt)query.append("created_at, ");
        if (columns & NoteColumn::updatedAt)query.append("updated_at, ");
        query.removeAt(query.length() - 2);
        query.append(
            QString(
                "FROM notes WHERE NOT id = ? AND id NOT IN (SELECT target_id FROM note_links WHERE source_id = ?) AND id")
            %
            (invert ? " NOT" : "") %
            " IN (SELECT rowid FROM notes_fts WHERE title LIKE ? ESCAPE '\\' OR body LIKE ? ESCAPE '\\');");


        param.replace(QRegularExpression("([%_])"), "\\\\1");
        param.prepend("%");
        param.append("%");
        SQLite::Statement statement(*space->getFile(), query.toStdString());
        statement.bind(1, sourceId);
        statement.bind(2, sourceId);
        statement.bind(3, param.toStdString());
        statement.bind(4, param.toStdString());
        return statement;
    }
} // NTA
