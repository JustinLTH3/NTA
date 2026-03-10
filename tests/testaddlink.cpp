//
// Created by JustinLai on 1/3/2026.
//

#include "testaddlink.h"

#include <spdlog/spdlog.h>

#include "../src/nspacemanager.h"
#include "../src/space.h"

void TestAddLink::init()
{
    space.reset();
    QDir::current().remove("test.db");
    space.reset(NTA::Space::createSpace(QDir::current(), "test.db"));
    NTA::NNoteManager::createInstance(space);
    QVERIFY(space != nullptr);
    for (int i = 1; i <= 10; ++i)
    {
        NTA::NNoteManager::getInstance()->createNote(1, QString::number(i));
    }
}

void TestAddLink::cleanup()
{
    {
        NTA::NNoteManager::destroyInstance();
        space.reset();
    }
    QDir::current().remove("test.db");
}

void TestAddLink::addLinkTest()
{
    QVERIFY(space->addLink(1, 2));
    SQLite::Statement query(*space->getFile(), "SELECT * FROM note_links");
    QVERIFY(query.executeStep());
    QVERIFY(query.getColumn("source_id").getInt64() == 1);
    QVERIFY(query.getColumn("target_id").getInt64() == 2);
    QVERIFY(!query.executeStep());
}

void TestAddLink::addAlreadyExistingLinkTest()
{
    QVERIFY(space->addLink(1, 2));
    QVERIFY(!space->addLink(1, 2));
}

void TestAddLink::addLinkToNonExistingNoteTest()
{
    QVERIFY(!space->addLink(100, 2));
    QVERIFY(!space->addLink(1, 100));
    QVERIFY(!space->addLink(100, 100));
}

void TestAddLink::addReverseLinkTest()
{
    QVERIFY(space->addLink(1, 2));
    QVERIFY(space->addLink(2, 1));
}

void TestAddLink::addLinkToSelfTest()
{
    QVERIFY(!space->addLink(1, 1));
}

void TestAddLink::addMultipleLinksFromSameSourceTest()
{
    QVERIFY(space->addLink(1, 2));
    QVERIFY(space->addLink(1, 3));
    QVERIFY(space->addLink(1, 4));
}

void TestAddLink::addMultipleLinksToSameTargetTest()
{
    QVERIFY(space->addLink(1, 2));
    QVERIFY(space->addLink(3, 2));
    QVERIFY(space->addLink(4, 2));
}
