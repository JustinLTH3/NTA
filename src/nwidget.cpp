//
// Created by JustinLai on 4/3/2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NWidget.h" resolved

#include "nwidget.h"

#include <QLayout>
#include <QPointer>
#include <QTimer>
#include <spdlog/spdlog.h>

#include "DockAreaTitleBar.h"
#include "DockAreaWidget.h"
#include "DockContainerWidget.h"
#include "DockManager.h"
#include "ElidingLabel.h"
#include "FloatingDockContainer.h"
#include "nspacemanager.h"
#include "nwidgetmanager.h"
#include "ui_NWidget.h"
#include "QAction"

namespace NTA
{
    NWidget::NWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent) : CDockWidget(dockManager,
            title, parent), ui(new Ui::NWidget)
    {
        ui->setupUi(this);
        connect(this, &ads::CDockWidget::topLevelChanged, this, &NWidget::onFloat);
        auto a = new QAction(tr("pin"), this);
        connect(a, &QAction::triggered, this, &NWidget::togglePin);
        linkCurrentAction = new QAction(tr("link"), this);
        linkCurrentAction->setCheckable(true);
        connect(linkCurrentAction, &QAction::triggered, this, &NWidget::linkCurrent);
        setTitleBarActions({a, linkCurrentAction});

        connect(NNoteManager::getInstance(), &NNoteManager::noteUpdated, this, &NWidget::onNoteUpdated);
    }

    NWidget::~NWidget()
    {
        delete ui;
    }

    QSharedPointer<Note> NWidget::getNote() const
    {
        return note;
    }

    bool NWidget::getIsLinked() const
    {
        return isLinked;
    }

    void NWidget::linkNote(const QSharedPointer<Note>& inNote, bool linked)
    {
        setNote(inNote);
        isLinked = linked && inNote;
        linkCurrentAction->setChecked(isLinked);
    }

    void NWidget::setNote(const QSharedPointer<Note>& inNote)
    {
        if (inNote && inNote == note)return;
        note = inNote;
    }

    void NWidget::linkCurrent(bool l)
    {
        if (l && note)
        {
            isLinked = true;
        }
        else if (l != isLinked)
        {
            isLinked = false;
            setNote(NNoteManager::getInstance()->getNoteWithId(NWidgetManager::getInstance()->currentNoteId));
        }

        linkCurrentAction->setChecked(isLinked);
    }


    void NWidget::togglePin()
    {
        if (floatingDockContainer())
        {
            floatingDockContainer()->setWindowFlag(Qt::WindowStaysOnTopHint,
                                                   (floatingDockContainer()->windowFlags() &
                                                       Qt::WindowStaysOnTopHint) == 0);
            //Widget will be hidden if not call for widgets within the floating dock container.
            for (auto w : floatingDockContainer()->dockWidgets())
                w->toggleView(true);
            floatingDockContainer()->show();
            //Delay as sometimes not successfully raise the widget if call immediate.
            QTimer::singleShot(0, this, [this]()
            {
                floatingDockContainer()->raise();
                floatingDockContainer()->activateWindow();
            });
        }
    }

    void NWidget::onFloat(bool isFloating)
    {
        spdlog::info("on float");
        if (isFloating && floatingDockContainer())
        {
            //Remove parent for not bringing up other floating widgets.
            floatingDockContainer()->setParent(nullptr);
        }
    }
} // NTA
