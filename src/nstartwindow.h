#ifndef NTA_NSTARTWINDOW_H
#define NTA_NSTARTWINDOW_H

#include <QMainWindow>

namespace NTA
{
    QT_BEGIN_NAMESPACE

    namespace Ui
    {
        class NStartWindow;
    }

    QT_END_NAMESPACE

    class NStartWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit NStartWindow(QWidget* parent = nullptr);
        ~NStartWindow() override;

    private:
        Ui::NStartWindow* ui;
    };
} // NTA

#endif //NTA_NSTARTWINDOW_H
