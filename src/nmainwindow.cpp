#include "nmainwindow.h"

#include <spdlog/spdlog.h>

#include "DockAreaWidget.h"
#include "ui_NMainWindow.h"
#include "DockManager.h"
#include "nwidget.h"

namespace NTA
{
    NMainWindow::NMainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::NMainWindow)
    {
        ui->setupUi(this);
        ads::CDockManager::setConfigFlag(ads::CDockManager::FocusHighlighting, true);
        dockManager = new ads::CDockManager();
        ads::CDockManager::setConfigFlag(ads::CDockManager::AlwaysShowTabs, true);
        dockContainer = new ads::CDockContainerWidget(dockManager);
        setCentralWidget(dockContainer);
        auto widget = new NWidget(dockManager, "Test");
        auto f = dockManager->addDockWidgetFloating(widget);
        f->show();
        widget = new NWidget(dockManager, "Test2");
        f = dockManager->addDockWidgetFloating(widget);
        f->show();
        widget = new NWidget(dockManager, "Test3");
        f = dockManager->addDockWidgetFloating(widget);
        f->show();
    }

    NMainWindow::~NMainWindow()
    {
        delete ui;
    }
} // NTA
