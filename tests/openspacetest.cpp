#include "openspacetest.h"
#include "openspacetest.moc"
#include "../src/space.h"

void OpenSpaceTest::init()
{
    dir = QDir::current();
    QScopedPointer space(NTA::Space::createSpace(dir, "test.db"));
}

void OpenSpaceTest::openExistingSpaceTest()
{
    const QScopedPointer space1(NTA::Space::openExistingSpace(dir.path() + "/test.db"));
    QVERIFY(space1 != nullptr);
    QVERIFY(space1->getFile() != nullptr);
}

void OpenSpaceTest::openNonExistingSpaceTest()
{
    const QScopedPointer space(NTA::Space::openExistingSpace(dir.path() + "/test2.db"));
    QVERIFY(space == nullptr);
}

void OpenSpaceTest::openNullSpaceTest()
{
    const QScopedPointer space(NTA::Space::openExistingSpace(""));
    QVERIFY(space == nullptr);
}

void OpenSpaceTest::openDatabaseWithWrongHeaderTest()
{
    QScopedPointer space(NTA::Space::openExistingSpace(dir.path() + "/test.db"));
    QVERIFY(space != nullptr);
    space->getFile()->exec("PRAGMA application_id = 1;");
    const QScopedPointer space2(NTA::Space::openExistingSpace(dir.path() + "/test.db"));
    QVERIFY(space2 == nullptr);
}

void OpenSpaceTest::cleanup()
{
    dir.remove("test.db");
}

void OpenSpaceTest::cleanupTestCase()
{
}
