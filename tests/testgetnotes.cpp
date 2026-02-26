#include "testgetnotes.h"
#include "testgetnotes.moc"
#include "../src/space.h"
#include "../src/note.h"

void TestGetNotes::init()
{
    space.reset(NTA::Space::createSpace(QDir::current(), "test.db"));
    QVERIFY(space != nullptr);
}

void TestGetNotes::getNoteWithIdTest()
{
    QString title = "First note";
    auto createdNote = space->createNote(1, title);
    auto note = space->getNoteWithId(createdNote->id);

    QVERIFY(note != nullptr);
    QVERIFY(note->title == title);
    QVERIFY(note->typeId == 1);
    QVERIFY(note->id == createdNote->id);
    QVERIFY(note->createdAt.isValid());
    QVERIFY(note->updatedAt.isValid());
}

void TestGetNotes::getNonExistingNoteTest()
{
    auto note = space->getNoteWithId(100);
    QVERIFY(note == nullptr);
}

void TestGetNotes::searchNoteByTextTest()
{
    QString title = "Note: ";
    QSet<int64_t> notes;
    for (int i = 1; i <= 10; ++i)
    {
        notes.insert(i);
        space->createNote(1, title + QString::number(i));
    }
    auto query = space->searchNotes(title);
    while (query.executeStep())
    {
        QVERIFY(notes.contains(query.getColumn("id").getInt64()));
        notes.remove(query.getColumn("id").getInt64());
    }
    QVERIFY(notes.isEmpty());
}

void TestGetNotes::searchNoteBySpecificTextTest()
{
    QString title = "Note: ";
    for (int i = 1; i <= 10; ++i)
    {
        space->createNote(1, title + QString::number(i));
    }
    auto query = space->searchNotes("1");
    int count = 0;
    while (query.executeStep())
    {
        QVERIFY(query.getColumn("id").getInt64()==1 || query.getColumn("id").getInt64()==10);
        ++count;
    }
    QVERIFY(count == 2);
}

void TestGetNotes::cleanup()
{
    space.reset();
    QDir::current().remove("test.db");
}
