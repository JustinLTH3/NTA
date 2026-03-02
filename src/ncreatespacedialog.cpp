//
// Created by JustinLai on 2/3/2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NCreateSpaceDialog.h" resolved

#include "ncreatespacedialog.h"
#include "ui_NCreateSpaceDialog.h"

namespace NTA
{
    NCreateSpaceDialog::NCreateSpaceDialog(QWidget* parent) : QDialog(parent), ui(new Ui::NCreateSpaceDialog)
    {
        ui->setupUi(this);
    }

    NCreateSpaceDialog::~NCreateSpaceDialog()
    {
        delete ui;
    }
} // NTA