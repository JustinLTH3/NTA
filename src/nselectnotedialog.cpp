//
// Created by JustinLai on 18/3/2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NSelectNoteDialog.h" resolved

#include "nselectnotedialog.h"

#include <QPointer>
#include <spdlog/spdlog.h>
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
        ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        auto n = NLinkManager::getInstance()->searchNotesExcludeLinked(
            "", id, NoteColumn::id | NoteColumn::title);
        while (n.executeStep())
        {
            auto item = new QListWidgetItem(QString::fromStdString(n.getColumn("title").getString()), ui->listWidget);
            ui->listWidget->addItem(item);
            items[item] = n.getColumn("id").getInt64();
            ids[items[item]] = item;
        }
        timer.setSingleShot(true);
        connect(ui->lineEdit, &QLineEdit::textChanged, this, [this]()
        {
            if (timer.isActive())return;
            timer.start(1000);
        });
        connect(&timer, &QTimer::timeout, this, [this]()
        {
            auto text = ui->lineEdit->text();
            auto l = NLinkManager::getInstance()->
                    searchNotesExcludeLinked(text, this->id, NoteColumn::id);
            while (ui->listWidget->count())
            {
                ui->listWidget->takeItem(0);
            }
            while (l.executeStep())
            {
                ui->listWidget->addItem(ids[l.getColumn("id").getInt64()]);
            }
        });
    }

    NSelectNoteDialog::~NSelectNoteDialog()
    {
        delete ui;
        timer.stop();
        for (auto item: ids)
        {
            delete item;
        }
    }

    QList<int64_t> NSelectNoteDialog::getSelected()
    {
        auto i = ui->listWidget->selectedItems();
        QList<int64_t> output;
        for (auto item: this->ids)
        {
            if (item->isSelected())
                output.append(items[item]);
        }
        return output;
    }
} // NTA
