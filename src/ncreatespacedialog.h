#ifndef NTA_NCREATESPACEDIALOG_H
#define NTA_NCREATESPACEDIALOG_H

#include <QDialog>
#include <QDir>

namespace NTA
{
    QT_BEGIN_NAMESPACE

    namespace Ui
    {
        class NCreateSpaceDialog;
    }

    QT_END_NAMESPACE

    class NCreateSpaceDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit NCreateSpaceDialog(QWidget* parent = nullptr);
        ~NCreateSpaceDialog() override;
        QDir getPath() const;
        QString getSpaceName() const;

    private:
        Ui::NCreateSpaceDialog* ui;
    };
} // NTA

#endif //NTA_NCREATESPACEDIALOG_H
