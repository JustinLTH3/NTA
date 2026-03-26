//
// Created by JustinLai on 22/3/2026.
//

#pragma once
#include "md4c.h"
#include "nwidget.h"

class QTextBrowser;

namespace NTA
{
    class nmd;

    class NRenderWidget : public NWidget
    {
        Q_OBJECT

    public:
        NRenderWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent);
        void setNote(const QSharedPointer<Note>& inNote) override;

        unsigned flags = MD_DIALECT_GITHUB | MD_FLAG_WIKILINKS | MD_FLAG_LATEXMATHSPANS | MD_FLAG_TASKLISTS;

    protected slots:
        void onFocusNoteChanged(int64_t old, int64_t now) override;
        void onNoteUpdated(int64_t id, NWidget* from, unsigned int columns) override;
        void onAnchorClicked(const QUrl& url);
    protected:
        QTextBrowser* textBrowser;
        QSharedPointer<nmd> mdParser;
    };
} // NTA
