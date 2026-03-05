#include "testnwidget.h"

#include "NTestWidget.h"
#include "QMainWindow"
#include "src/DockManager.h"
#include "../src/note.h"
#include "QObject"


void TestNWidget::initTestCase()
{
    m_MainWindow = new QMainWindow;
    m_DockManager = new ads::CDockManager(m_MainWindow);
    m_MainWindow->setCentralWidget(m_DockManager);
    m_MainWindow->show();
    QVERIFY(m_MainWindow);
    QVERIFY(m_DockManager);
    QVERIFY(m_DockManager->parent() == m_MainWindow);
}

void TestNWidget::init()
{
}

void TestNWidget::widgetInitTest()
{
    NTestWidget* widget = new NTestWidget(m_DockManager, "Test", m_MainWindow);
    QVERIFY(widget != nullptr);
    QVERIFY(widget->parent() == m_MainWindow);
    QVERIFY(widget->dockManager() == m_DockManager);
    auto a = widget->titleBarActions();
    QVERIFY(a.size() == 1);
    widget->deleteLater();
}

void TestNWidget::widgetLinkToNoteTest()
{
    NTestWidget* widget = new NTestWidget(m_DockManager, "Test", m_MainWindow);
    QVERIFY(widget != nullptr);
    QSharedPointer<NTA::Note> note = QSharedPointer<NTA::Note>(new NTA::Note{.id = 1, .title = "Test", .body = "Test"});
    widget->linkNote(note);
    QVERIFY(widget->getNote() == note);
    QVERIFY(widget->getIsLinked());
    widget->deleteLater();
}

void TestNWidget::widgetPinTest()
{
    NTestWidget* widget = new NTestWidget(m_DockManager, "Test", m_MainWindow);

    auto f = m_DockManager->addDockWidgetFloating(widget);
    f->show();
    auto a = widget->titleBarActions();
    auto c = f->windowFlags();
    QVERIFY(a.size() == 1);
    a[0]->trigger();
    QTest::qWait(100);
    QVERIFY(f->isVisible());
    QVERIFY(widget->isVisible());
    QVERIFY(f->windowFlags() & Qt::WindowStaysOnTopHint);
    QVERIFY((c & f->windowFlags()) == c);
    widget->deleteLater();
}

void TestNWidget::widgetUnpinTest()
{
    NTestWidget* widget = new NTestWidget(m_DockManager, "Test", m_MainWindow);

    auto f = m_DockManager->addDockWidgetFloating(widget);
    f->show();
    auto a = widget->titleBarActions();
    auto c = f->windowFlags();
    QVERIFY(a.size() == 1);
    a[0]->trigger();
    QTest::qWait(100);

    a[0]->trigger();
    QTest::qWait(100);

    QVERIFY(f->isVisible());
    QVERIFY(widget->isVisible());
    QVERIFY((f->windowFlags() & Qt::WindowStaysOnTopHint)== 0);
    QVERIFY(c == f->windowFlags());

    widget->deleteLater();
}

void TestNWidget::cleanup()
{
}

void TestNWidget::cleanupTestCase()
{
    delete m_MainWindow;
}
