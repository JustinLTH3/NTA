//
// Created by JustinLai on 1/3/2026.
//

#include "testaddlink.h"

#include <spdlog/spdlog.h>

#include "../src/nlinkmanager.h"
#include "../src/nspacemanager.h"
#include "../src/space.h"

void LinkManagerTest::init()
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

void LinkManagerTest::cleanup()
{
    //
    {
        NTA::NNoteManager::destroyInstance();
        NTA::NLinkManager::destroyInstance();
        space.reset();
    }
    QDir::current().remove("test.db");
}

void LinkManagerTest::addLinkTest()
{
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 2));
    SQLite::Statement query(*space->getFile(), "SELECT * FROM note_links");
    QVERIFY(query.executeStep());
    QVERIFY(query.getColumn("source_id").getInt64() == 1);
    QVERIFY(query.getColumn("target_id").getInt64() == 2);
    QVERIFY(!query.executeStep());
}

void LinkManagerTest::addAlreadyExistingLinkTest()
{
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 2));
    QVERIFY(!NTA::NLinkManager::getInstance()->addLink(1, 2));
}

void LinkManagerTest::addLinkToNonExistingNoteTest()
{
    QVERIFY(!NTA::NLinkManager::getInstance()->addLink(100, 2));
    QVERIFY(!NTA::NLinkManager::getInstance()->addLink(1, 100));
    QVERIFY(!NTA::NLinkManager::getInstance()->addLink(100, 100));
}

void LinkManagerTest::addReverseLinkTest()
{
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 2));
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(2, 1));
}

void LinkManagerTest::addLinkToSelfTest()
{
    QVERIFY(!NTA::NLinkManager::getInstance()->addLink(1, 1));
}

void LinkManagerTest::addMultipleLinksFromSameSourceTest()
{
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 2));
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 3));
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 4));
}

void LinkManagerTest::addMultipleLinksToSameTargetTest()
{
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 2));
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(3, 2));
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(4, 2));
}

void LinkManagerTest::getLinksTest()
{
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 2));
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(2, 1));
    auto links = NTA::NLinkManager::getInstance()->getLinks(1);
    QVERIFY(links.executeStep());
    QVERIFY(links.getColumn("source_id").getInt64() == 1);
    QVERIFY(links.getColumn("target_id").getInt64() == 2);
    QVERIFY(!links.executeStep());
}

void LinkManagerTest::removeLinkTest()
{
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 2));
    QVERIFY(NTA::NLinkManager::getInstance()->removeLink(1, 2));
    auto links = NTA::NLinkManager::getInstance()->getLinks(1);
    QVERIFY(!links.executeStep());
}

void LinkManagerTest::removeNonExistingLinkTest()
{
}

void LinkManagerTest::searchLinkTest()
{
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 2));
    QVERIFY(NTA::NLinkManager::getInstance()->editLink(1, 2, "Test description", "Test alias"));
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(2, 1));
    QVERIFY(NTA::NLinkManager::getInstance()->editLink(2, 1, "Test description%", "Test alias_"));

    auto links = NTA::NLinkManager::getInstance()->searchLinks("Test");
    QVERIFY(links.executeStep());
    QVERIFY(links.executeStep());
    QVERIFY(!links.executeStep());
    links = NTA::NLinkManager::getInstance()->searchLinks("description%");
    QVERIFY(links.executeStep());
    QVERIFY(links.getColumn("source_id").getInt64() == 2);
    QVERIFY(links.getColumn("target_id").getInt64() == 1);
    QVERIFY(!links.executeStep());
    links = NTA::NLinkManager::getInstance()->searchLinks("alias_");
    QVERIFY(links.executeStep());
    QVERIFY(links.getColumn("source_id").getInt64() == 2);
    QVERIFY(links.getColumn("target_id").getInt64() == 1);
    QVERIFY(!links.executeStep());
}

void LinkManagerTest::searchLinkFromSourceTest()
{
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 2));
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(2, 1));
    QVERIFY(NTA::NLinkManager::getInstance()->editLink(1, 2, "Test description", "Test alias"));
    QVERIFY(NTA::NLinkManager::getInstance()->editLink(2, 1, "Test description", "Test alias"));
    auto links = NTA::NLinkManager::getInstance()->searchLinksOfSourceWithNoteTitle("Test", 1);
    QVERIFY(links.executeStep());
    QVERIFY(links.getColumn("source_id").getInt64() == 1);
    QVERIFY(links.getColumn("target_id").getInt64() == 2);
    QVERIFY(!links.executeStep());
    links = NTA::NLinkManager::getInstance()->searchLinksOfSourceWithNoteTitle("Test", 2);
    QVERIFY(links.executeStep());
    QVERIFY(links.getColumn("source_id").getInt64() == 2);
    QVERIFY(links.getColumn("target_id").getInt64() == 1);
    QVERIFY(!links.executeStep());
}

void LinkManagerTest::searchLinkWithEmptyParamsTest()
{
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 2));
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(2, 1));
    QVERIFY(NTA::NLinkManager::getInstance()->editLink(1, 2, "Test description", "Test alias"));
    QVERIFY(NTA::NLinkManager::getInstance()->editLink(2, 1, "Test description", "Test alias"));
    auto links = NTA::NLinkManager::getInstance()->searchLinks("");
    QVERIFY(links.executeStep());
    QVERIFY(links.executeStep());
    QVERIFY(!links.executeStep());
}

void LinkManagerTest::searchNotesExcludeLinkTest()
{
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(1, 2));
    QVERIFY(NTA::NLinkManager::getInstance()->addLink(2, 1));
    auto links = NTA::NLinkManager::getInstance()->searchNotesExcludeLinked("", 1);
    int count = 0;
    while (links.executeStep())
    {
        auto id = links.getColumn("id").getInt64();
        QVERIFY(id != 1);
        QVERIFY(id != 2);
        ++count;
    }
    QVERIFY(count == 8);
    links = NTA::NLinkManager::getInstance()->searchNotesExcludeLinked("1", 1);
    QVERIFY(links.executeStep());
    QVERIFY(links.getColumn("id").getInt64() == 10);
    QVERIFY(!links.executeStep());
}
