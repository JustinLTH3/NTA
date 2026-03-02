#include "ncreatespacedialog.h"

#include <QDir>
#include <QFileDialog>

#include "ui_NCreateSpaceDialog.h"

namespace NTA
{
    NCreateSpaceDialog::NCreateSpaceDialog(QWidget* parent) : QDialog(parent), ui(new Ui::NCreateSpaceDialog)
    {
        ui->setupUi(this);
        ui->dirEdit->setText(QDir::homePath());
        ui->lineEdit->setText("space");
        ui->lineEdit->setFocus();

        connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
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
