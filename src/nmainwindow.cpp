#include "nmainwindow.h"

#include <QMenuBar>
#include <QMessageBox>
#include <spdlog/spdlog.h>

#include "DockAreaWidget.h"
#include "ui_NMainWindow.h"
#include "DockManager.h"
#include "nnoteexplorer.h"
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

        getWindowMenu()->addAction("Note Explorer", this, [this]()
        {
            spdlog::info("show note explorer");
            NNoteExplorer* note_explorer = new NNoteExplorer(dockManager, "Note Explorer", this);
            dockContainer->addDockWidget(ads::AllDockAreas, note_explorer, dockContainer->dockArea(0));
        });
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
