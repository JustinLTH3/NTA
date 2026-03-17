//
// Created by JustinLai on 15/3/2026.
//

#include "nlinkmanager.h"

#include <SQLiteCpp/Statement.h>
#include "space.h"
#include <QPointer>
#include <qregularexpression.h>
#include <spdlog/spdlog.h>

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
                                    "INSERT OR IGNORE INTO note_links (source_id, target_id, alias, description) VALUES (?, ?, null, null)");
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
} // NTA
