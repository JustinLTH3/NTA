//
// Created by JustinLai on 19/3/2026.
//

#include "nbacklinkwidget.h"

#include <QListWidget>
#include <QPointer>
#include <QVBoxLayout>
#include <SQLiteCpp/Column.h>

#include "nlinkmanager.h"
#include "nspacemanager.h"

namespace NTA
{
    NBacklinkWidget::NBacklinkWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent) : NWidget(
        dockManager, title, parent)
    {
        auto central = new QWidget(this);
        setWidget(central);
        central->setLayout(new QVBoxLayout(central));
        listWidget = new QListWidget(central);
        central->layout()->addWidget(listWidget);
    }

    NBacklinkWidget::~NBacklinkWidget()
    {
    }

    void NBacklinkWidget::linkNote(const QSharedPointer<Note>& inNote, bool linked)
    {
        NWidget::linkNote(inNote, linked);
    }

    void NBacklinkWidget::setNote(const QSharedPointer<Note>& inNote)
    {
        NWidget::setNote(inNote);
        listWidget->clear();
        if (note)
        {
            auto links = NLinkManager::getInstance()->getBackLinks(note->id);
            while (links.executeStep())
            {
                listWidget->addItem(QString::fromStdString(links.getColumn("title").getString()));
            }
        }
    }

    void NBacklinkWidget::onFocusNoteChanged(int64_t old, int64_t now)
    {
        if (isLinked || old == now)return;
        setNote(NNoteManager::getInstance()->getNoteWithId(now));
    }

    void NBacklinkWidget::onNoteUpdated(int64_t id, NWidget* from, unsigned int columns)
    {
        NWidget::onNoteUpdated(id, from, columns);
    }
} // NTA
