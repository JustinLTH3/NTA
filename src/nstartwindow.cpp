#include "nstartwindow.h"
#include "ui_NStartWindow.h"
#include "QFileDialog"
#include <spdlog/spdlog.h>

#include "ncreatespacedialog.h"
#include "space.h"

namespace NTA
{
    NStartWindow::NStartWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::NStartWindow)
    {
        ui->setupUi(this);
        connect(ui->createBtn, &QPushButton::clicked, this, [this]()
        {
            NCreateSpaceDialog dialog(this);
            if (dialog.exec())
            {
                QSharedPointer<Space> space(Space::createSpace(dialog.getPath(), dialog.getSpaceName() + ".nta"));
                if (space)
                {
                    spdlog::info("create space success");
                    emit openSpace(space);
                }
            }
        });
    }

    NStartWindow::~NStartWindow()
    {
        delete ui;
    }
} // NTA
