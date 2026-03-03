#include "ncreatespacedialog.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include "ui_NCreateSpaceDialog.h"

namespace NTA
{
    NCreateSpaceDialog::NCreateSpaceDialog(QWidget* parent) : QDialog(parent), ui(new Ui::NCreateSpaceDialog)
    {
        ui->setupUi(this);
        ui->dirEdit->setText(QDir::homePath());
        ui->lineEdit->setText("space");
        ui->lineEdit->setFocus();

        connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]()
        {
            QDir dir(ui->dirEdit->text());
            if (!dir.exists() || ui->dirEdit->text().isEmpty())
            {
                QMessageBox::warning(this, tr("Warning"), tr("Directory not exist"));
                return;
            }
            if (dir.exists(ui->lineEdit->text() + ".nta"))
            {
                QMessageBox::warning(this, tr("Warning"), tr("Space already exist"));
                return;
            }
            this->accept();
        });
        connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
        connect(ui->findBtn, &QPushButton::clicked, this, [this]()
        {
            ui->dirEdit->setText(QFileDialog::getExistingDirectory(this, tr("Select directory"), ui->dirEdit->text()));
        });
    }

    NCreateSpaceDialog::~NCreateSpaceDialog()
    {
        delete ui;
    }

    QDir NCreateSpaceDialog::getPath() const
    {
        return QDir(ui->dirEdit->text());
    }

    QString NCreateSpaceDialog::getSpaceName() const
    {
        return ui->lineEdit->text();
    }
} // NTA
