//
// Created by JustinLai on 8/3/2026.
//

#include "spacemanagertest.h"

#include "../src/nspacemanager.h"
#include "../src/space.h"

void SpaceManagerTest::initTestCase()
{
    space.reset(NTA::Space::createSpace(QDir::current(), "space.nta"));
    QVERIFY(space != nullptr);
}

void SpaceManagerTest::createInstanceTest()
{
    QVERIFY(NTA::NSpaceManager::createInstance(space));
    QVERIFY(NTA::NSpaceManager::getInstance());
}

void SpaceManagerTest::createInstanceWithNullSpaceTest()
{
    QVERIFY(!NTA::NSpaceManager::createInstance(nullptr));
    QVERIFY(!NTA::NSpaceManager::getInstance());
}

void SpaceManagerTest::createInstanceWhenAlreadyExistTest()
{
    QVERIFY(NTA::NSpaceManager::createInstance(space));
    QVERIFY(!NTA::NSpaceManager::createInstance(space));
    QVERIFY(NTA::NSpaceManager::getInstance());
    NTA::NSpaceManager::destroyInstance();
}

void SpaceManagerTest::destroyInstanceTest()
{
    NTA::NSpaceManager::destroyInstance();
    QVERIFY(!NTA::NSpaceManager::getInstance());
}

void SpaceManagerTest::cleanupTestCase()
{
    NTA::NSpaceManager::destroyInstance();
    space.reset();
    if (QDir::current().exists("space.nta"))
        (QDir::current().remove("space.nta"));
}
