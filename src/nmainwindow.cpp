#include "nmainwindow.h"
#include "ui_NMainWindow.h"
#include "DockManager.h"

namespace NTA
{
    NMainWindow::NMainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::NMainWindow)
    {
        ui->setupUi(this);
        dockManager = new ads::CDockManager();
        dockContainer = new ads::CDockContainerWidget(dockManager);
        setCentralWidget(dockContainer);
        auto dock = dockManager->createDockWidget("Testing Dock");
        auto f = dockManager->addDockWidgetFloating(dock);
        f->show();
    }

    NMainWindow::~NMainWindow()
    {
        delete ui;
    }
} // NTA
