#include "nmainwindow.h"

#include <QMenuBar>
#include <QMessageBox>
#include <spdlog/spdlog.h>

#include "DockAreaWidget.h"
#include "ui_NMainWindow.h"
#include "DockManager.h"
#include "neditorwidget.h"
#include "nlinkwidget.h"
#include "nnoteexplorer.h"
#include "nwidget.h"
#include "nwidgetmanager.h"

namespace NTA
{
    NMainWindow::NMainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::NMainWindow)
    {
        ui->setupUi(this);
        ads::CDockManager::setConfigFlag(ads::CDockManager::FocusHighlighting, true);
        dockManager = new ads::CDockManager();
        ads::CDockManager::setConfigFlag(ads::CDockManager::AlwaysShowTabs, true);
        ads::CDockManager::setConfigFlag(ads::CDockManager::FloatingContainerHasWidgetTitle, true);
        dockContainer = new ads::CDockContainerWidget(dockManager);
        setCentralWidget(dockContainer);
        auto aboutQt = new QAction(tr("About Qt"), this);
        auto aboutNta = new QAction(tr("About NTA"), this);
        getHelpMenu()->addAction(aboutQt);
        getHelpMenu()->addAction(aboutNta);
        connect(aboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
        connect(aboutNta, &QAction::triggered, this, [this]()
        {
            QMessageBox::about(this, tr("About NTA"),
                               tr(
                                   "NTA is a open source note taking app with Markdown support. Placeholder..................."));
        });

        NWidgetManager::createInstance(dockManager);

        getWindowMenu()->addAction("Note Explorer", this, [this]()
        {
            spdlog::info("show note explorer");
            auto note_explorer = NWidgetManager::getInstance()->createWidget<NNoteExplorer>("Note Explorer", this);
            dockContainer->addDockWidget(ads::DockWidgetArea::CenterDockWidgetArea, note_explorer,
                                         dockContainer->dockArea(0));
        });
        getWindowMenu()->addAction("Link", this, [this]()
        {
            spdlog::info("show link");
            auto link = NWidgetManager::getInstance()->createWidget<NLinkWidget>("Link", this);
            dockContainer->addDockWidget(ads::DockWidgetArea::CenterDockWidgetArea, link,
                                         dockContainer->dockArea(0));
        });
        connect(dockManager, &ads::CDockManager::floatingWidgetCreated, this,
                [](ads::CFloatingDockContainer* floatingWidget)
                {
                    spdlog::info("floating widget created");
                    floatingWidget->setParent(nullptr);
                });
    }

    NMainWindow::~NMainWindow()
    {
        delete ui;
        NWidgetManager::destroyInstance();
        NNoteManager::destroyInstance();
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
