//
// Created by JustinLai on 8/3/2026.
//

#include "nspacemanager.h"

#include <QPointer>

namespace NTA
{
    QPointer<NSpaceManager> NSpaceManager::instance = nullptr;

    QPointer<NSpaceManager> NSpaceManager::getInstance()
    {
        return instance;
    }

    void NSpaceManager::destroyInstance()
    {
        if (instance)instance->deleteLater();
        instance = nullptr;
    }

    bool NSpaceManager::createInstance(const QSharedPointer<Space>& in_space)
    {
        if (!instance)
        {
            instance = new NSpaceManager(in_space);
            return true;
        }
        return false;
    }

    NSpaceManager::NSpaceManager(const QSharedPointer<Space>& in_space) : QObject(nullptr), space(in_space)
    {
    }
}
