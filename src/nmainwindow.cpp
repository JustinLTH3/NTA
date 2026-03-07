#include "nmainwindow.h"

#include <QMenuBar>
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
    }

    NMainWindow::~NMainWindow()
    {
        delete ui;
    }

    QMenu* NMainWindow::getFileMenu() const
    {
        return ui->menu_File;
    }

    QMenu* NMainWindow::getEditMenu() const
    {
        return ui->menu_Edit;
    }

    QMenu* NMainWindow::getSettingsMenu() const
    {
        return ui->menu_Settings;
    }

    QMenu* NMainWindow::getWindowMenu() const
    {
        return ui->menu_Window;
    }

    QMenu* NMainWindow::getHelpMenu() const
    {
        return ui->menuHelp;
    }
} // NTA
