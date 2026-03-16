//
// Created by JustinLai on 15/3/2026.
//

#include "nlinkmanager.h"

#include <SQLiteCpp/Statement.h>
#include "space.h"
#include <QPointer>

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
                                    "INSERT OR IGNORE INTO note_links (source_id, target_id) VALUES (?, ?)");
        statement.bind(1, from);
        statement.bind(2, to);
        return statement.exec();
    }
} // NTA
