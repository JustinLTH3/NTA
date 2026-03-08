#include "nnoteexplorer.h"

#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>

#include "ElidingLabel.h"

namespace NTA
{
    NNoteExplorer::NNoteExplorer(ads::CDockManager* dockManager, const QString& title,
                                 QWidget* parent) : NWidget(dockManager, title, parent)
    {
        auto central = new QWidget(this);
        setWidget(central);
        central->setLayout(new QVBoxLayout(central));
        central->layout()->addWidget(new QLineEdit(central));
        central->layout()->addWidget(new QListWidget(central));
    }

    NNoteExplorer::~NNoteExplorer()
    {
    }

    void NNoteExplorer::onFocusNoteChanged(const QSharedPointer<Note>& newNote)
    {
    }
} // NTA
