#include "nstartwindow.h"
#include "ui_NStartWindow.h"
#include "QFileDialog"
#include <spdlog/spdlog.h>

#include "ncreatespacedialog.h"

namespace NTA
{
    NStartWindow::NStartWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::NStartWindow)
    {
        ui->setupUi(this);
        connect(ui->createBtn, &QPushButton::clicked, this, [this]()
        {
            NCreateSpaceDialog dialog(this);
            dialog.exec();
        });
    }

    NStartWindow::~NStartWindow()
    {
        delete ui;
    }
} // NTA
