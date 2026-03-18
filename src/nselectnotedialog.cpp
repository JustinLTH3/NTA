//
// Created by JustinLai on 18/3/2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NSelectNoteDialog.h" resolved

#include "nselectnotedialog.h"

#include <QPointer>
#include <SQLiteCpp/Column.h>

#include "nlinkmanager.h"
#include "ui_NSelectNoteDialog.h"

namespace NTA
{
    NSelectNoteDialog::NSelectNoteDialog(int64_t id, QWidget* parent) : QDialog(parent), id(id),
                                                                        ui(new Ui::NSelectNoteDialog)
    {
        ui->setupUi(this);

        connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

        auto n = NLinkManager::getInstance()->searchNotesExcludeLinked(
            "", id, NoteColumn::id | NoteColumn::title);
        while (n.executeStep())
        {
            auto item = new QListWidgetItem(QString::fromStdString(n.getColumn("title").getString()), ui->listWidget);
            ui->listWidget->addItem(item);
            items[item] = n.getColumn("id").getInt64();
        }
    }

    NSelectNoteDialog::~NSelectNoteDialog()
    {
        delete ui;
    }

    QList<int64_t> NSelectNoteDialog::getSelected()
    {
        auto i = ui->listWidget->selectedItems();
        QList<int64_t> ids;
        for (auto item : i)
        {
            ids.append(items[item]);
        }
        return ids;
    }
} // NTA
