#pragma once
#include <QTest>

namespace ads
{
    class CDockManager;
}

class QMainWindow;

class TestNWidget : public QObject
{
    Q_OBJECT

protected:
    QMainWindow* m_MainWindow = nullptr;
    ads::CDockManager* m_DockManager = nullptr;

private slots:
    void initTestCase();
    void init();
    void widgetInitTest();
    void widgetLinkToNoteTest();
    void widgetPinTest();
    void widgetUnpinTest();
    void cleanup();
    void cleanupTestCase();
};
