//
// Created by JustinLai on 22/3/2026.
//

#include "nrenderwidget.h"

#include <QPointer>
#include <QScrollBar>
#include <qtextbrowser.h>
#include <QTextEdit>
#include <QVBoxLayout>
#include <spdlog/spdlog.h>
#include <SQLiteCpp/Column.h>

#include "md4c-html.h"
#include "nlinkmanager.h"
#include "nmd.h"
#include "note.h"
#include "nspacemanager.h"

namespace NTA
{
    NRenderWidget::NRenderWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent)
        : NWidget(dockManager, title, parent)
    {
        static QWeakPointer<nmd> mdP;
        if (mdP.isNull())
        {
            mdParser = QSharedPointer<nmd>(new nmd);
            mdP = mdParser;
        }
        else
        {
            mdParser = mdP.toStrongRef();
        }
        auto central = new QWidget(this);
        setWidget(central);
        central->setLayout(new QVBoxLayout(central));
        textBrowser = new QTextBrowser();
        central->layout()->addWidget(textBrowser);
        textBrowser->setOpenLinks(false);
        connect(textBrowser, &QTextBrowser::anchorClicked, this, &NRenderWidget::onAnchorClicked);
    }

    void NRenderWidget::setNote(const QSharedPointer<Note>& inNote)
    {
        NWidget::setNote(inNote);
        if (inNote)
        {
            if (QString s; !mdParser->md2html(inNote->body, s))
                textBrowser->setHtml(s);
        }
    }

    void NRenderWidget::onFocusNoteChanged(int64_t old, int64_t now)
    {
        if (isLinked || old == now)return;
        setNote(NNoteManager::getInstance()->getNoteWithId(now));
    }

    void NRenderWidget::onNoteUpdated(int64_t id, NWidget* from, unsigned int columns)
    {
        if (id != note->id)return;
        if (columns & NoteColumn::body)
        {
            if (QString s; !mdParser->md2html(note->body, s))
            {
                int v = textBrowser->verticalScrollBar()->value();
                textBrowser->setHtml(s);
                textBrowser->verticalScrollBar()->setValue(v);
            }
        }
    }

    void NRenderWidget::onAnchorClicked(const QUrl& url)
    {
        if (url.scheme() == "nta")
        {
            auto links = NLinkManager::getInstance()->getLinks(note->id);
            while (links.executeStep())
            {
                if (links.getColumn("alias").getString() == url.path().toStdString())
                {
                    linkNote(NNoteManager::getInstance()->getNoteWithId(links.getColumn("target_id").getInt64()));
                    return;
                }
            }
        }
    }
} // NTA
