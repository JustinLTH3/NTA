//
// Created by JustinLai on 18/3/2026.
//

#ifndef NTA_NSELECTNOTEDIALOG_H
#define NTA_NSELECTNOTEDIALOG_H

#include <QDialog>
#include <qlistwidget.h>

namespace NTA
{
    QT_BEGIN_NAMESPACE

    namespace Ui
    {
        class NSelectNoteDialog;
    }

    QT_END_NAMESPACE

    class NSelectNoteDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit NSelectNoteDialog(int64_t id, QWidget* parent = nullptr);
        ~NSelectNoteDialog() override;
        QList<int64_t> getSelected();
    protected:
        int64_t id;
        QMap<QListWidgetItem*, int64_t> items;
    private:
        Ui::NSelectNoteDialog* ui;
    };
} // NTA

#endif //NTA_NSELECTNOTEDIALOG_H
