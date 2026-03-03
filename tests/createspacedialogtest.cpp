#include "createspacedialogtest.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <QSignalSpy>

#include "../src/ncreatespacedialog.h"
#include <QLineEdit>
#include <spdlog/spdlog.h>

void CreateSpaceDialogTest::initTestCase()
{
}

void CreateSpaceDialogTest::init()
{
    dialog = new NTA::NCreateSpaceDialog();
    QVERIFY(dialog != nullptr);
}

void CreateSpaceDialogTest::cleanup()
{
    dialog->deleteLater();
}

void CreateSpaceDialogTest::createSpaceDialogTest()
{
    auto dirEdit = dialog->findChild<QLineEdit*>("dirEdit");
    QVERIFY(dirEdit != nullptr);
    QVERIFY(dirEdit->text() == QDir::homePath());
    QTest::keyClick(dirEdit, Qt::Key_A, Qt::ControlModifier); // Ctrl+A
    QTest::keyClick(dirEdit, Qt::Key_Backspace);
    QTest::keyClicks(dirEdit, QDir::currentPath());
    QVERIFY(dirEdit->text() == QDir::currentPath());
    auto buttonBox = dialog->findChild<QDialogButtonBox*>("buttonBox");
    QVERIFY(buttonBox != nullptr);
    QSignalSpy acceptSpy(buttonBox, &QDialogButtonBox::accepted);
    QTest::mouseClick(qobject_cast<QWidget*>(buttonBox->button(QDialogButtonBox::Ok)), Qt::LeftButton);
    QVERIFY(acceptSpy.count() == 1);
}

void CreateSpaceDialogTest::createSpaceDialogWhenDirNotExistTest()
{
    auto dirEdit = dialog->findChild<QLineEdit*>("dirEdit");
    QVERIFY(dirEdit != nullptr);
    QTest::keyClick(dirEdit, Qt::Key_A, Qt::ControlModifier); // Ctrl+A
    QTest::keyClick(dirEdit, Qt::Key_Backspace);
    auto buttonBox = dialog->findChild<QDialogButtonBox*>("buttonBox");
    QSignalSpy acceptSpy(dialog, &QDialog::accepted);
    QTimer::singleShot(50, [=]()
    {
        QWidget* widget = QApplication::activeModalWidget();
        if (widget)
            widget->close();
    });
    QTest::mouseClick(qobject_cast<QWidget*>(buttonBox->button(QDialogButtonBox::Ok)), Qt::LeftButton);
    QVERIFY(acceptSpy.count() == 0);
}
