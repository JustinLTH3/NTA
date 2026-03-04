#ifndef NTA_NWIDGET_H
#define NTA_NWIDGET_H

#include "DockWidget.h"

namespace NTA
{
    QT_BEGIN_NAMESPACE

    namespace Ui
    {
        class NWidget;
    }

    QT_END_NAMESPACE

    class NWidget : public ads::CDockWidget
    {
        Q_OBJECT

    public:
        explicit NWidget(const QString& title, QWidget* parent = nullptr);
        ~NWidget() override;

    private:
        Ui::NWidget* ui;
    };
} // NTA

#endif //NTA_NWIDGET_H
