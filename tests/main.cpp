#include "spacetest.h"

int main(int argc, char* argv[])
{
    SpaceTest test1;
    QTest::qExec(&test1, argc, argv);
    return 0;
}
