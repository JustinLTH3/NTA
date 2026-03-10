//
// Created by JustinLai on 5/3/2026.
//

#include "NTestWidget.h"

#include "ElidingLabel.h"
#include "src/DockManager.h"

NTestWidget::NTestWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent) : NWidget(
    dockManager, title, parent)
{
    setWidget(new QLabel(tr("NTestWidget"), this));
}

void NTestWidget::onFocusNoteChanged(int64_t old, int64_t now)
{
}
