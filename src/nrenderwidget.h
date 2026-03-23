//
// Created by JustinLai on 22/3/2026.
//

#pragma once
#include "md4c.h"
#include "nwidget.h"

namespace NTA
{
    class NRenderWidget : public NWidget
    {
        Q_OBJECT

    public:
        NRenderWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent);

    protected slots:
        void onFocusNoteChanged(int64_t old, int64_t now) override;
        void onNoteUpdated(int64_t id, NWidget* from, unsigned int columns) override;
    };
} // NTA
