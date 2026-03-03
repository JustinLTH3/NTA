#pragma once
#include <QTest>
#include <QtWidgets>


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
