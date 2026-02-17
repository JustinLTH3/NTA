#include "testcreatenote.h"

#include "../src/space.h"
#include "../src/note.h"
#include "testcreatenote.moc"

void TestCreateNote::init()
{
    space.reset(NTA::Space::createSpace(QDir::current(), "test.db"));
    QVERIFY(space != nullptr);
}

void TestCreateNote::createDefaultNoteTest()
{
    auto note = space->createNote();
    QVERIFY(note != nullptr);
    QVERIFY(note->typeId == 1);
    QVERIFY(note->title == "New note");
    QVERIFY(note->body.isEmpty());
    QVERIFY(note->id == 1);
    QVERIFY(note->createdAt.isValid());
    QVERIFY(note->updatedAt.isValid());
    QVERIFY(note->createdAt == note->updatedAt);
    SQLite::Statement query(*space->getFile(), "SELECT * FROM notes WHERE id = ?");
    query.bind(1, note->id);
    QVERIFY(query.executeStep());
    QVERIFY(!query.executeStep());
}

void TestCreateNote::createNoteWithTitleTest()
{
    auto note = space->createNote(1, "Other Title");
    QVERIFY(note != nullptr);
    QVERIFY(note->typeId == 1);
    QVERIFY(note->title == "Other Title");
    QVERIFY(note->body.isEmpty());
    QVERIFY(note->createdAt.isValid());
    QVERIFY(note->updatedAt.isValid());
    QVERIFY(note->createdAt == note->updatedAt);
    SQLite::Statement query(*space->getFile(), "SELECT * FROM notes WHERE id = ?");
    query.bind(1, note->id);
    QVERIFY(query.executeStep());
    QVERIFY(!query.executeStep());
}

void TestCreateNote::createNoteWithOtherTypeIdTest()
{
    auto note = space->createNote(2);
    QVERIFY(note != nullptr);
    QVERIFY(note->typeId == 2);
    SQLite::Statement query(*space->getFile(), "SELECT * FROM notes WHERE id = ?");
    query.bind(1, note->id);
    QVERIFY(query.executeStep());
}

void TestCreateNote::createNoteWithInvalidTypeIdTest()
{
    auto note = space->createNote(3);
    QVERIFY(note == nullptr);
}

void TestCreateNote::cleanup()
{
    space.reset();
    QDir::current().remove("test.db");
}
