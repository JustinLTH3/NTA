#ifndef NTA_NMAINWINDOW_H
#define NTA_NMAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE

namespace Ui
{
    class NMainWindow;
}

QT_END_NAMESPACE

class NMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NMainWindow(QWidget* parent = nullptr);
    ~NMainWindow() override;

private:
    Ui::NMainWindow* ui;
};


#endif //NTA_NMAINWINDOW_H
