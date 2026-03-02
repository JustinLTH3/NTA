//
// Created by JustinLai on 2/3/2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NStartWindow.h" resolved

#include "nstartwindow.h"
#include "ui_NStartWindow.h"

namespace NTA
{
    NStartWindow::NStartWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::NStartWindow)
    {
        ui->setupUi(this);
    }

    NStartWindow::~NStartWindow()
    {
        delete ui;
    }
} // NTA