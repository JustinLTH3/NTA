//
// Created by JustinLai on 12/3/2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NEditorWidget.h" resolved

#include "neditorwidget.h"

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPointer>
#include <QVBoxLayout>
#include "note.h"
#include "nspacemanager.h"

namespace NTA
{
    NEditorWidget::NEditorWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent) : NWidget(
        dockManager, title, parent)
    {
        auto central = new QWidget(this);
        setWidget(central);
        central->setLayout(new QVBoxLayout(central));
        titleBar = new QLineEdit(central);
        central->layout()->addWidget(titleBar);
        content = new QPlainTextEdit(central);
        central->layout()->addWidget(content);
        saveTimer.setSingleShot(true);
        connect(&saveTimer, &QTimer::timeout, this, [this]()
        {
            NNoteManager::getInstance()->updateNote(
                note->id, Note{.title = titleBar->text(), .body = content->toPlainText()}, this,
                NoteColumn::title | NoteColumn::body);
        });
        connect(titleBar, &QLineEdit::textChanged, this, [this](const QString& text)
        {
            if (!note)return;
            if (saveTimer.isActive())return;
            saveTimer.start(1000);
        });
        connect(content, &QPlainTextEdit::textChanged, this, [this]()
        {
            if (!note)return;
            if (saveTimer.isActive())return;
            saveTimer.start(1000);
        });
    }

    NEditorWidget::~NEditorWidget()
    {
    }

    void NEditorWidget::linkNote(const QSharedPointer<Note>& inNote, bool linked)
    {
        NWidget::linkNote(inNote, linked);
        if (note)
        {
            titleBar->setText(note->title);
            content->setPlainText(note->body);
        }
    }

    void NEditorWidget::setNote(const QSharedPointer<Note>& inNote)
    {
        NWidget::setNote(inNote);
    }

    void NEditorWidget::onFocusNoteChanged(int64_t old, int64_t now)
    {
    }

    void NEditorWidget::onNoteUpdated(int64_t id, NWidget* from, unsigned int columns)
    {
        if (from == this)return;
        if (note && columns & NoteColumn::title)titleBar->setText(note->title);
        if (note && columns & NoteColumn::body)content->setPlainText(note->body);
    }
} // NTA
