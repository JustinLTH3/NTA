#include "startwindowtest.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <qtimer.h>
#include <QLineEdit>

#include "../src/ncreatespacedialog.h"
#include "../src/nstartwindow.h"

void StartWindowTest::init()
{
    window = new NTA::NStartWindow();
    QVERIFY(window != nullptr);
}

void StartWindowTest::cleanup()
{
    window->deleteLater();
}

void StartWindowTest::createSpaceTest()
{
    auto createSpaceButton = window->findChild<QPushButton*>("createBtn");
    QVERIFY(createSpaceButton != nullptr);
    QTimer::singleShot(0, [=, this]()
    {
        auto d = window->findChild<NTA::NCreateSpaceDialog*>();
        QVERIFY(d != nullptr);
        auto dirEdit = d->findChild<QLineEdit*>("dirEdit");
        QVERIFY(dirEdit != nullptr);
        QTest::keyClick(dirEdit, Qt::Key_A, Qt::ControlModifier); // Ctrl+A
        QTest::keyClick(dirEdit, Qt::Key_Backspace);
        QTest::keyClicks(dirEdit, QDir::currentPath());
        QTest::mouseClick(
            qobject_cast<QWidget*>(d->findChild<QDialogButtonBox*>("buttonBox")->button(QDialogButtonBox::Ok)),
            Qt::LeftButton);
    });
    QTest::mouseClick(createSpaceButton, Qt::LeftButton);
    QVERIFY(QDir::current().exists("space.nta"));
    QDir::current().remove("space.nta");
}

void StartWindowTest::createSpaceWhenFailTest()
{
    auto createSpaceButton = window->findChild<QPushButton*>("createBtn");
    QVERIFY(createSpaceButton != nullptr);
    QTimer::singleShot(0, [=, this]()
    {
        auto d = window->findChild<NTA::NCreateSpaceDialog*>();
        QVERIFY(d != nullptr);
        auto dirEdit = d->findChild<QLineEdit*>("dirEdit");
        QVERIFY(dirEdit != nullptr);
        QTest::keyClick(dirEdit, Qt::Key_A, Qt::ControlModifier); // Ctrl+A
        QTest::keyClick(dirEdit, Qt::Key_Backspace);
        QTimer::singleShot(0, [=]()
        {
            QWidget* widget = QApplication::activeModalWidget();
            if (widget)
                widget->close();
        });
        QTest::mouseClick(
            qobject_cast<QWidget*>(d->findChild<QDialogButtonBox*>("buttonBox")->button(QDialogButtonBox::Ok)),
            Qt::LeftButton);
        QVERIFY(d->isVisible());
        QTest::mouseClick(
            qobject_cast<QWidget*>(d->findChild<QDialogButtonBox*>("buttonBox")->button(QDialogButtonBox::Cancel)),
            Qt::LeftButton);
    });
    QTest::mouseClick(createSpaceButton, Qt::LeftButton);
    QVERIFY(!QDir::current().exists("space.nta"));
}
