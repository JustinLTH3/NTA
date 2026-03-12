//
// Created by JustinLai on 10/3/2026.
//

#include "nwidgetmanager.h"

#include <QPointer>
#include <QXmlStreamReader>

#include "note.h"
#include "DockManager.h"

namespace NTA
{
    QPointer<NWidgetManager> NWidgetManager::getInstance()
    {
        return instance;
    }

    void NWidgetManager::destroyInstance()
    {
        if (instance)instance->deleteLater();
        instance = nullptr;
    }

    bool NWidgetManager::createInstance(const QPointer<ads::CDockManager>& dockManager)
    {
        if (!instance && dockManager)
        {
            instance = new NWidgetManager(dockManager);
            return true;
        }
        return false;
    }

    NWidgetManager::NWidgetManager(const QPointer<ads::CDockManager>& dockManager, QObject* parent) : QObject(parent),
        dockManager(dockManager)
    {
        connect(dockManager, &ads::CDockManager::focusedDockWidgetChanged, this, [this](ads::CDockWidget* old,
            ads::CDockWidget* now)
                {
                    auto n = qobject_cast<NWidget*>(now);
                    if (!n)
                        return;
                    if (!n->note && currentNoteId == -1)return;
                    if (currentNoteId == n->note->id) return;
                    emit currentFocusedNoteChanged(currentNoteId, n->note->id);
                    currentNoteId = n->note->id;
                });
    }

    NWidgetManager::~NWidgetManager()
    {
    }

    void NWidgetManager::removeWidget(NWidget* widget)
    {
        widgets[widget->typeName].removeAll(widget);
        widget->deleteLater();
    }

    QPointer<NWidgetManager> NWidgetManager::instance = nullptr;
} // NTA
