#include "QTest"


//
#include "spacetest.h"
#include "spacetest.moc"

#include <spdlog/spdlog.h>
#include "../src/space.h"

void SpaceTest::initTestCase()
{
    dir = QDir::current();
}

void SpaceTest::createSpaceTest()
{
    const QScopedPointer space(NTA::Space::createSpace(dir, "test.db"));
    QVERIFY(space != nullptr);
    QVERIFY(space->getFile() != nullptr);
    QVERIFY(dir.exists("test.db"));
}

void SpaceTest::createSpaceWhenSpaceExistTest()
{
    QScopedPointer space(NTA::Space::createSpace(dir, "test.db"));
    QVERIFY(space != nullptr);
    QVERIFY(space->getFile() != nullptr);
    space.reset();
    space.reset(NTA::Space::createSpace(dir, "test.db"));
    QVERIFY(space == nullptr);
}

void SpaceTest::createSpaceWhenEmptyFileNameTest()
{
    const QScopedPointer space(NTA::Space::createSpace(dir, ""));
    QVERIFY(space == nullptr);
}

void SpaceTest::createSpaceWhenDirNotExistTest()
{
    QDir dir2 = dir.path() + "/test2";
    QVERIFY(!dir2.exists());
    const QScopedPointer space(NTA::Space::createSpace(dir2, "test.db"));
    QVERIFY(space == nullptr);
}


void SpaceTest::cleanup()
{
    if (dir.exists("test.db"))
        dir.remove("test.db");
}
