#ifndef NTA_NMAINWINDOW_H
#define NTA_NMAINWINDOW_H

#include <QMainWindow>
#include <QPointer>

#include "DockManager.h"

namespace NTA
{
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

    protected:
        QPointer<ads::CDockManager> dockManager;
        QPointer<ads::CDockContainerWidget> dockContainer;

    private:
        Ui::NMainWindow* ui;
    };
} // NTA

#endif //NTA_NMAINWINDOW_H
