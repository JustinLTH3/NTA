#include "createspacedialogtest.h"
#include "createspacetest.h"
#include "nwidgetmanagertest.h"
#include "openspacetest.h"
#include "spacemanagertest.h"
#include "startwindowtest.h"
#include "testaddlink.h"
#include "testcreatenote.h"
#include "testgetnotes.h"
#include "testnwidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    CreateSpaceTest test1;
    QTest::qExec(&test1, argc, argv);
    OpenSpaceTest test2;
    QTest::qExec(&test2, argc, argv);
    TestCreateNote test3;
    QTest::qExec(&test3, argc, argv);
    TestGetNotes test4;
    QTest::qExec(&test4, argc, argv);
    TestAddLink test5;
    QTest::qExec(&test5, argc, argv);
    CreateSpaceDialogTest test6;
    QTest::qExec(&test6, argc, argv);
    StartWindowTest test7;
    QTest::qExec(&test7, argc, argv);
    TestNWidget test8;
    QTest::qExec(&test8, argc, argv);
    NoteManagerTest test9;
    QTest::qExec(&test9, argc, argv);
    NWidgetManagerTest test10;
    QTest::qExec(&test10, argc, argv);
    return 0;
}
