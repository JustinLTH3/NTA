//
// Created by JustinLai on 15/3/2026.
//

#include "nlinkwidget.h"

#include <QComboBox>
#include <QLineEdit>
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

        removeBtn = new QToolButton();
        central->layout()->addWidget(removeBtn);
        removeBtn->setText("Remove");

        connect(addBtn, &QToolButton::clicked, this, [this]()
        {
            if (!note)return;
            NSelectNoteDialog selectNoteDialog(note->id);
            if (selectNoteDialog.exec())
            {
                auto s = selectNoteDialog.getSelected();
                for (auto& i : s)
                {
                    NLinkManager::getInstance()->addLink(note->id, i);
                    auto l = NLinkManager::getInstance()->getLink(note->id, i);
                    if (l.executeStep())
                        listWidget->addItem(QString::fromStdString(l.getColumn("alias").getString()));
                }
            }
        });

        connect(removeBtn, &QToolButton::clicked, this, [this]()
        {
            if (!note)return;
            auto sItems = listWidget->selectedItems();
            if (sItems.isEmpty())return;
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
            auto links = NLinkManager::getInstance()->searchLinksOfSourceWithNoteTitle("", note->id);
            while (links.executeStep())
            {
                auto* item = new QListWidgetItem(listWidget);
                auto* widget = new QWidget(listWidget);
                listWidget->addItem(item);
                widget->setLayout(new QVBoxLayout(widget));
                auto id = links.getColumn("target_id").getInt64();
                auto title = new QLabel(widget);
                title->setText(QString::fromStdString(links.getColumn("title").getString()));
                auto alias = new QLineEdit(widget);
                alias->setText(QString::fromStdString(links.getColumn("alias")));
                auto description = new QLineEdit(widget);
                description->setText(QString::fromStdString(links.getColumn("description")));
                widget->layout()->addWidget(title);
                widget->layout()->addWidget(alias);
                widget->layout()->addWidget(description);
                connect(alias, &QLineEdit::textEdited, alias, [this, id, alias, description](auto& text)
                {
                    NLinkManager::getInstance()->editLink(note->id, id, description->text(), text);
                });
                connect(description, &QLineEdit::textEdited, description, [this, id, alias, description](auto& text)
                {
                    NLinkManager::getInstance()->editLink(note->id, id, text, alias->text());
                });
                item->setSizeHint(widget->sizeHint());
                listWidget->setItemWidget(item, widget);
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
