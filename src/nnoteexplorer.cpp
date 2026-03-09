#include "nnoteexplorer.h"

#include <QLineEdit>
#include <QListWidget>
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
        central->layout()->addWidget(new QLineEdit(central));
        auto createNoteBtn = new QPushButton("Create Note", central);
        central->layout()->addWidget(createNoteBtn);
        connect(createNoteBtn, &QPushButton::clicked, this, [this]()
        {
            spdlog::info("create note");
            NSpaceManager::getInstance()->getSpace()->createNote();
        });
        listWidget = new QListWidget(central);
        central->layout()->addWidget(listWidget);
        auto l = NSpaceManager::getInstance()->getSpace()->searchNotes("");
        while (l.executeStep())
        {
            listWidget->addItem(
                NSpaceManager::getInstance()->getSpace()->getNoteWithId(l.getColumn("id").getInt64())->title);
        }
    }

    NNoteExplorer::~NNoteExplorer()
    {
    }

    void NNoteExplorer::onFocusNoteChanged(const QSharedPointer<Note>& newNote)
    {
    }
} // NTA
