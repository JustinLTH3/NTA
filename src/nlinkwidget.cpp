//
// Created by JustinLai on 15/3/2026.
//

#include "nlinkwidget.h"

#include <QVBoxLayout>

#include "ElidingLabel.h"

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
    }

    void NLinkWidget::onFocusNoteChanged(int64_t old, int64_t now)
    {

    }

    void NLinkWidget::onNoteUpdated(int64_t id, NWidget* from, unsigned int columns)
    {
        NWidget::onNoteUpdated(id, from, columns);
    }
} // NTA
