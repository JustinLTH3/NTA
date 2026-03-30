//
// Created by JustinLai on 8/3/2026.
//

#include "spacemanagertest.h"

#include "../src/nspacemanager.h"
#include "../src/space.h"

void NoteManagerTest::initTestCase()
{
    space.reset(NTA::Space::createSpace(QDir::current(), "space.nta"));
    QVERIFY(space != nullptr);
}

void NoteManagerTest::createInstanceTest()
{
    QVERIFY(NTA::NNoteManager::createInstance(space));
    QVERIFY(NTA::NNoteManager::getInstance());
}

void NoteManagerTest::createInstanceWithNullSpaceTest()
{
    QVERIFY(!NTA::NNoteManager::createInstance(nullptr));
    QVERIFY(!NTA::NNoteManager::getInstance());
}

void NoteManagerTest::createInstanceWhenAlreadyExistTest()
{
    QVERIFY(NTA::NNoteManager::createInstance(space));
    QVERIFY(!NTA::NNoteManager::createInstance(space));
    QVERIFY(NTA::NNoteManager::getInstance());
    NTA::NNoteManager::destroyInstance();
}

void NoteManagerTest::destroyInstanceTest()
{
    NTA::NNoteManager::destroyInstance();
    QVERIFY(!NTA::NNoteManager::getInstance());
}

void NoteManagerTest::cleanupTestCase()
{
    NTA::NNoteManager::destroyInstance();
    space.reset();
    if (QDir::current().exists("space.nta"))
        (QDir::current().remove("space.nta"));
}
