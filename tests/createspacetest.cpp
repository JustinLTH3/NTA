#include "QTest"


//
#include "createspacetest.h"
#include "createspacetest.moc"

#include <spdlog/spdlog.h>
#include "../src/space.h"

void CreateSpaceTest::initTestCase()
{
    dir = QDir::current();
}

void CreateSpaceTest::createSpaceTest()
{
    const QScopedPointer space(NTA::Space::createSpace(dir, "test.db"));
    QVERIFY(space != nullptr);
    QVERIFY(space->getFile() != nullptr);
    QVERIFY(dir.exists("test.db"));
}

void CreateSpaceTest::createSpaceWhenSpaceExistTest()
{
    QScopedPointer space(NTA::Space::createSpace(dir, "test.db"));
    QVERIFY(space != nullptr);
    QVERIFY(space->getFile() != nullptr);
    space.reset();
    space.reset(NTA::Space::createSpace(dir, "test.db"));
    QVERIFY(space == nullptr);
}

void CreateSpaceTest::createSpaceWhenEmptyFileNameTest()
{
    const QScopedPointer space(NTA::Space::createSpace(dir, ""));
    QVERIFY(space == nullptr);
}

void CreateSpaceTest::createSpaceWhenDirNotExistTest()
{
    QDir dir2 = dir.path() + "/test2";
    QVERIFY(!dir2.exists());
    const QScopedPointer space(NTA::Space::createSpace(dir2, "test.db"));
    QVERIFY(space == nullptr);
}


void CreateSpaceTest::cleanup()
{
    if (dir.exists("test.db"))
        dir.remove("test.db");
}
