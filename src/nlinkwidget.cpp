//
// Created by JustinLai on 15/3/2026.
//

#include "nlinkwidget.h"

#include <QPointer>
#include <QToolButton>
#include <QVBoxLayout>
#include <SQLiteCpp/Column.h>

#include "ElidingLabel.h"
#include "nlinkmanager.h"
#include "nselectnotedialog.h"
#include "nspacemanager.h"
#include "PushButton.h"

namespace NTA
{
    NLinkWidget::NLinkWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent) : NWidget(
        dockManager, title, parent)
    {
        auto central = new QWidget(this);
        setWidget(central);
        central->setLayout(new QVBoxLayout(central));
        listWidget = new QListWidget(central);
        central->layout()->addWidget(listWidget);
        addBtn = new QToolButton();
        central->layout()->addWidget(addBtn);
        addBtn->setText("Add");
        connect(addBtn, &QToolButton::clicked, this, [this]()
        {
            if (!note)return;
            NSelectNoteDialog selectNoteDialog(note->id);
            if (selectNoteDialog.exec())
            {
                auto s = selectNoteDialog.getSelected();
                for (auto& i: s)
                {
                    NLinkManager::getInstance()->addLink(note->id, i);
                }
            }
        });
    }

    NLinkWidget::~NLinkWidget()
    {
    }

    void NLinkWidget::linkNote(const QSharedPointer<Note>& inNote, bool linked)
    {
        NWidget::linkNote(inNote, linked);
    }

    void NLinkWidget::setNote(const QSharedPointer<Note>& inNote)
    {
        NWidget::setNote(inNote);
        listWidget->clear();
        if (note)
        {
            auto links = NLinkManager::getInstance()->getLinks(note->id);
            while (links.executeStep())
            {
                listWidget->addItem(QString::fromStdString(links.getColumn("alias").getString()));
            }
        }
    }

    void NLinkWidget::onFocusNoteChanged(int64_t old, int64_t now)
    {
        if (isLinked || old == now)return;
        setNote(NNoteManager::getInstance()->getNoteWithId(now));
    }

    void NLinkWidget::onNoteUpdated(int64_t id, NWidget* from, unsigned int columns)
    {
        NWidget::onNoteUpdated(id, from, columns);
    }
} // NTA
