#include "openspacetest.h"
#include "openspacetest.moc"
#include "../src/space.h"

void OpenSpaceTest::initTestCase()
{
    dir = QDir::current();
    if (dir.exists("test.db"))
        dir.remove("test.db");
    QScopedPointer space(NTA::Space::createSpace(dir, "test.db"));
}

void OpenSpaceTest::openExistingSpaceTest()
{
    const QScopedPointer space(NTA::Space::createSpace(dir, "test.db"));
    const QScopedPointer space2(NTA::Space::openExistingSpace(dir.path() + "/test.db"));
    QVERIFY(space2 != nullptr);
    QVERIFY(space2->getFile() != nullptr);
}

void OpenSpaceTest::openNonExistingSpaceTest()
{
    const QScopedPointer space(NTA::Space::openExistingSpace(dir.path() + "/test2.db"));
    QVERIFY(space == nullptr);
}

void OpenSpaceTest::cleanup()
{
}

void OpenSpaceTest::cleanupTestCase()
{
    if (dir.exists("test.db"))
        dir.remove("test.db");
}
