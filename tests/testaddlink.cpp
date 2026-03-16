//
// Created by JustinLai on 1/3/2026.
//

#include "testaddlink.h"

#include <spdlog/spdlog.h>

#include "../src/nlinkmanager.h"
#include "../src/nspacemanager.h"
#include "../src/space.h"

void TestAddLink::init()
{
    space.reset();
    QDir::current().remove("test.db");
    space.reset(NTA::Space::createSpace(QDir::current(), "test.db"));
    NTA::NNoteManager::createInstance(space);
    NTA::NLinkManager::createInstance(space);
    QVERIFY(space != nullptr);
    QVERIFY(NTA::NLinkManager::getInstance() != nullptr);
    QVERIFY(NTA::NNoteManager::getInstance() != nullptr);
    for (int i = 1; i <= 10; ++i)
    {
        NTA::NNoteManager::getInstance()->createNote(1, QString::number(i));
    }
}

void TestAddLink::cleanup()
{
    //
    {
        NTA::NNoteManager::destroyInstance();
        NTA::NLinkManager::destroyInstance();
        space.reset();
    }
    QDir::current().remove("test.db");
}

void TestAddLink::addLinkTest()
{
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 2));
    SQLite::Statement query(*space->getFile(), "SELECT * FROM note_links");
    QVERIFY(query.executeStep());
    QVERIFY(query.getColumn("source_id").getInt64() == 1);
    QVERIFY(query.getColumn("target_id").getInt64() == 2);
    QVERIFY(!query.executeStep());
}

void TestAddLink::addAlreadyExistingLinkTest()
{
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 2));
    QVERIFY(!NTA::NLinkManager::getInstance()->addLink(1, 2));
}

void TestAddLink::addLinkToNonExistingNoteTest()
{
    QVERIFY(!NTA::NLinkManager::getInstance()->addLink(100, 2));
    QVERIFY(!NTA::NLinkManager::getInstance()->addLink(1, 100));
    QVERIFY(!NTA::NLinkManager::getInstance()->addLink(100, 100));
}

void TestAddLink::addReverseLinkTest()
{
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 2));
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(2, 1));
}

void TestAddLink::addLinkToSelfTest()
{
    QVERIFY(!NTA::NLinkManager::getInstance()->addLink(1, 1));
}

void TestAddLink::addMultipleLinksFromSameSourceTest()
{
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 2));
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 3));
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 4));
}

void TestAddLink::addMultipleLinksToSameTargetTest()
{
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 2));
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(3, 2));
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(4, 2));
}
