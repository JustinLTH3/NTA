#include "nmainwindow.h"
#include "ui_NMainWindow.h"


NMainWindow::NMainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::NMainWindow)
{
    ui->setupUi(this);
}

NMainWindow::~NMainWindow()
{
    delete ui;
}
