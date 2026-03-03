#pragma once
#include <QTest>
#include <QtWidgets>


#include "createspacedialogtest.moc"
namespace NTA
{
    class NCreateSpaceDialog;
}

class CreateSpaceDialogTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void createSpaceDialogTest();
    void createSpaceDialogWhenDirIsEmptyTest();
    void createSpaceDialogWhenDirNotExistTest();
    void createSpaceDialogWhenAlreadyExistTest();
    void cleanup();
private:
    QPointer<NTA::NCreateSpaceDialog> dialog;
};
