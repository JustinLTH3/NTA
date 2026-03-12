//
// Created by JustinLai on 12/3/2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NEditorWidget.h" resolved

#include "neditorwidget.h"

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QVBoxLayout>

namespace NTA
{
    NEditorWidget::NEditorWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent) : NWidget(
        dockManager, title, parent)
    {
        auto central = new QWidget(this);
        setWidget(central);
        central->setLayout(new QVBoxLayout(central));
        auto titleBar = new QLineEdit(central);
        central->layout()->addWidget(titleBar);
        auto content = new QPlainTextEdit(central);
        central->layout()->addWidget(content);
    }

    NEditorWidget::~NEditorWidget()
    {
    }

    void NEditorWidget::linkNote(const QSharedPointer<Note>& inNote, bool linked)
    {
        NWidget::linkNote(inNote, linked);
    }

    void NEditorWidget::setNote(const QSharedPointer<Note>& inNote)
    {
        NWidget::setNote(inNote);
    }

    void NEditorWidget::onFocusNoteChanged(int64_t old, int64_t now)
    {
    }
} // NTA
