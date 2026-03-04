//
// Created by JustinLai on 4/3/2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NWidget.h" resolved

#include "nwidget.h"

#include <QLayout>
#include <QTimer>
#include <spdlog/spdlog.h>

#include "DockAreaTitleBar.h"
#include "DockAreaWidget.h"
#include "DockContainerWidget.h"
#include "DockManager.h"
#include "ElidingLabel.h"
#include "FloatingDockContainer.h"
#include "ui_NWidget.h"
#include "QAction"

namespace NTA
{
    NWidget::NWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent) : CDockWidget(dockManager,
            title, parent), ui(new Ui::NWidget)
    {
        ui->setupUi(this);
        connect(this, &ads::CDockWidget::topLevelChanged, this, &NWidget::onFloat);
        // auto b = new QPushButton("pin", this);
        // setTitleBarActions({b});
        // setWidget(b);
        setWidget(new QLabel("Testing"));
        auto a = new QAction(tr("pin"), this);
        setTitleBarActions({a});
        connect(a, &QAction::triggered, this, &NWidget::togglePin);
        // connect(b, &QPushButton::clicked, this, &NWidget::togglePin);
    }

    NWidget::~NWidget()
    {
        spdlog::info("NWidget::~NWidget");
        delete ui;
    }

    void NWidget::togglePin()
    {
        if (floatingDockContainer())
        {
            floatingDockContainer()->setWindowFlag(Qt::WindowStaysOnTopHint,
                                                   (floatingDockContainer()->windowFlags() &
                                                    Qt::WindowStaysOnTopHint) == 0);
            //Widget will be hidden if not call for widgets within the floating dock container.
            for (auto w: floatingDockContainer()->dockWidgets())
                w->toggleView(true);
            floatingDockContainer()->show();
            //Delay as sometimes not successfully raise the widget if call immediate.
            QTimer::singleShot(10, this, [this]()
            {
                floatingDockContainer()->raise();
            });
        }
    }

    void NWidget::onFloat(bool isFloating)
    {
        if (isFloating && floatingDockContainer())
        {
            //Remove parent for not bringing up other floating widgets.
            floatingDockContainer()->setParent(nullptr);
        }
    }
} // NTA
