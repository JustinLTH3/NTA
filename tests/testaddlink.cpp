//
// Created by JustinLai on 1/3/2026.
//

#include "testaddlink.h"
#include "../src/space.h"

void TestAddLink::init()
{
    space.reset(NTA::Space::createSpace(QDir::current(), "test.db"));
    QVERIFY(space != nullptr);
    for (int i = 1; i <= 10; ++i)
    {
        space->createNote(1, QString::number(i));
    }
}

void TestAddLink::cleanup()
{
    space.reset();
    QDir::current().remove("test.db");
}

void TestAddLink::addLinkTest()
{
    QVERIFY(space->addLink(1, 2));
    SQLite::Statement query(*space->getFile(), "SELECT * FROM note_links");
    QVERIFY(query.executeStep());
    QVERIFY(query.getColumn("source_id").getInt64() == 1);
    QVERIFY(query.getColumn("target_id").getInt64() == 2);
}
