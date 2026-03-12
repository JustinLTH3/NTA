#include "nnoteexplorer.h"

#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QPointer>
#include <QVBoxLayout>
#include <spdlog/spdlog.h>

#include "ElidingLabel.h"
#include "note.h"
#include "nspacemanager.h"
#include "PushButton.h"
#include "space.h"

namespace NTA
{
    NNoteExplorer::NNoteExplorer(ads::CDockManager* dockManager, const QString& title,
                                 QWidget* parent) : NWidget(dockManager, title, parent)
    {
        auto central = new QWidget(this);
        setWidget(central);
        central->setLayout(new QVBoxLayout(central));
        auto searchBar = new QLineEdit(central);
        central->layout()->addWidget(searchBar);
        auto createNoteBtn = new QPushButton("Create Note", central);
        central->layout()->addWidget(createNoteBtn);
        connect(createNoteBtn, &QPushButton::clicked, this, [this]()
        {
            spdlog::info("create note");
            auto note = NNoteManager::getInstance()->createNote();
            QListWidgetItem* item = new QListWidgetItem(note->title, listWidget);
            items[item] = note->id;
            listWidget->addItem(item);
        });
        listWidget = new QListWidget(central);
        central->layout()->addWidget(listWidget);
        auto l = NNoteManager::getInstance()->searchNotes("", NoteColumn::id | NoteColumn::title);
        while (l.executeStep())
        {
            QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(l.getColumn("title").getString()),
                                                        listWidget);
            items[item] = l.getColumn("id").getInt64();
            listWidget->addItem(item);
        }
        listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(listWidget, &QListWidget::customContextMenuRequested, this, [this](const QPoint& pos)
        {
            QPoint itemP = listWidget->mapToGlobal(pos);
            QMenu submenu;
            submenu.addAction("Delete");
            QAction* rightClickItem = submenu.exec(itemP);
            if (rightClickItem && rightClickItem->text().contains("Delete"))
            {
                auto row = listWidget->indexAt(pos).row();
                auto item = listWidget->item(row);
                NNoteManager::getInstance()->getSpace()->deleteNote(items[item]);
                items.remove(item);
                delete item;
            }
        });

        connect(searchBar, &QLineEdit::textChanged, this, [this](const QString& text)
        {
            auto l = NNoteManager::getInstance()->searchNotes(text, NoteColumn::id | NoteColumn::title);
            listWidget->clear();
            while (l.executeStep())
            {
                QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(l.getColumn("title").getString()),
                                                            listWidget);
                items[item] = l.getColumn("id").getInt64();
                listWidget->addItem(item);
            }
        });
    }

    NNoteExplorer::~NNoteExplorer()
    {
    }

    void NNoteExplorer::linkNote(const QSharedPointer<Note>& inNote, bool linked)
    {
    }


    void NNoteExplorer::setNote(const QSharedPointer<Note>& inNote)
    {
    }

    void NNoteExplorer::onFocusNoteChanged(int64_t old, int64_t now)
    {
    }
} // NTA
