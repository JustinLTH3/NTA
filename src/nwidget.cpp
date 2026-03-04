//
// Created by JustinLai on 4/3/2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NWidget.h" resolved

#include "nwidget.h"
#include "ui_NWidget.h"

namespace NTA
{
    NWidget::NWidget(const QString & title, QWidget* parent) : CDockWidget(title, parent), ui(new Ui::NWidget)
    {
        ui->setupUi(this);
    }

    NWidget::~NWidget()
    {
        delete ui;
    }
} // NTA