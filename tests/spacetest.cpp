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
    const QScopedPointer space(NTA::Space::createStorage(dir, "test.db"));
    QVERIFY(space != nullptr);
    QVERIFY(space->getFile() != nullptr);
}

void SpaceTest::createSpaceWhenSpaceExistTest()
{
    QScopedPointer space(NTA::Space::createStorage(dir, "test.db"));
    QVERIFY(space != nullptr);
    QVERIFY(space->getFile() != nullptr);
    space.reset();
    space.reset(NTA::Space::createStorage(dir, "test.db"));
    QVERIFY(space == nullptr);
}

void SpaceTest::cleanup()
{
    if (dir.exists("test.db"))
        dir.remove("test.db");
}
