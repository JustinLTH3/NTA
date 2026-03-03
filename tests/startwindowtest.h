#pragma once
#include <QTest>

namespace NTA
{
    class NStartWindow;
}

class StartWindowTest : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void createSpaceTest();
    void cleanup();

private:
    QPointer<NTA::NStartWindow> window;
};
