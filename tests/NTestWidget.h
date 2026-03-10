//
// Created by JustinLai on 5/3/2026.
//

#pragma once
#include <QObject>
#include "../src/nwidget.h"


namespace ads
{
    class CDockManager;
}

class NTestWidget : public NTA::NWidget
{
    Q_OBJECT

public:
    explicit NTestWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent = nullptr);

protected
slots :
    void onFocusNoteChanged(int64_t old, int64_t now) override;
};
