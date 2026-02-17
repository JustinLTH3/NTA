#include "createspacetest.h"
#include "openspacetest.h"
#include "testcreatenote.h"

int main(int argc, char* argv[])
{
    CreateSpaceTest test1;
    QTest::qExec(&test1, argc, argv);
    OpenSpaceTest test2;
    QTest::qExec(&test2, argc, argv);
    TestCreateNote test3;
    QTest::qExec(&test3, argc, argv);
    return 0;
}
