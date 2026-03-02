#include "createspacetest.h"
#include "openspacetest.h"
#include "testaddlink.h"
#include "testcreatenote.h"
#include "testgetnotes.h"

int main(int argc, char* argv[])
{
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
    return 0;
}
