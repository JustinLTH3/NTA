#include "testgetnotes.h"
#include "testgetnotes.moc"

#include <spdlog/spdlog.h>

#include "../src/space.h"
#include "../src/note.h"
#include "../src/nspacemanager.h"

void TestGetNotes::init()
{
    space.reset(NTA::Space::createSpace(QDir::current(), "test.db"));
    NTA::NNoteManager::createInstance(space);
    QVERIFY(space != nullptr);
}

void TestGetNotes::getNoteWithIdTest()
{
    QString title = "First note";
    auto createdNote = NTA::NNoteManager::getInstance()->createNote(1, title);
    auto note = NTA::NNoteManager::getInstance()->getNoteWithId(createdNote->id);

    QVERIFY(note != nullptr);
    QVERIFY(note->title == title);
    QVERIFY(note->typeId == 1);
    QVERIFY(note->id == createdNote->id);
    QVERIFY(note->createdAt.isValid());
    QVERIFY(note->updatedAt.isValid());
}

void TestGetNotes::getNonExistingNoteTest()
{
    auto note = NTA::NNoteManager::getInstance()->getNoteWithId(100);
    QVERIFY(note == nullptr);
}

void TestGetNotes::searchNoteByTextTest()
{
    QString title = "My Note: ";
    QSet<int64_t> notes;
    for (int i = 1; i <= 10; ++i)
    {
        notes.insert(i);
        NTA::NNoteManager::getInstance()->createNote(1, title + QString::number(i));
    }
    try
    {
        auto query = NTA::NNoteManager::getInstance()->searchNotes(title);
        while (query.executeStep())
        {
            QVERIFY(notes.contains(query.getColumn("id").getInt64()));
            notes.remove(query.getColumn("id").getInt64());
        }
    } catch (std::exception& e)
    {
        spdlog::error("{}", e.what());
    }
    QVERIFY(notes.count() == 0);
}

void TestGetNotes::searchNoteBySpecificTextTest()
{
    QString title = "Note: ";
    for (int i = 1; i <= 10; ++i)
    {
        NTA::NNoteManager::getInstance()->createNote(1, title + QString::number(i));
    }
    try
    {
        auto query = NTA::NNoteManager::getInstance()->searchNotes("1");
        int count = 0;
        while (query.executeStep())
        {
            QVERIFY(query.getColumn("id").getInt64()==1);
            ++count;
        }

        QVERIFY(count == 1);
    } catch (std::exception& e)
    {
        spdlog::error("{}", e.what());
        QVERIFY(false);
    }
}

void TestGetNotes::searchNoteByEmptyTextTest()
{
    for (int i = 1; i <= 10; ++i)
    {
        NTA::NNoteManager::getInstance()->createNote(1, QString::number(i));
    }
    try
    {
        auto query = NTA::NNoteManager::getInstance()->searchNotes("");
        int count = 0;
        while (query.executeStep())
        {
            ++count;
        }
        QVERIFY(count == 10);
    } catch (std::exception& e)
    {
        spdlog::error("{}", e.what());
    }
}

void TestGetNotes::searchNoteWithSpecificColumnTest()
{
    QString title = "Note: ";
    for (int i = 1; i <= 10; ++i)
    {
        NTA::NNoteManager::getInstance()->createNote(1, title + QString::number(i));
    }
    try
    {
        auto query = NTA::NNoteManager::getInstance()->searchNotes(title, NTA::NoteColumn::id | NTA::NoteColumn::title);
        int count = 0;
        QVERIFY(query.executeStep());
        QVERIFY(query.getColumnCount() == 2);
        QVERIFY(query.getColumn("id").getInt64() == 1);
        QVERIFY(query.getColumn("title") == title + QString::number(1));
    } catch (std::exception& e)
    {
        spdlog::error("{}", e.what());
        QVERIFY(false);
    }
}

void TestGetNotes::cleanup()
{
    space.reset();
    NTA::NNoteManager::destroyInstance();
    QDir::current().remove("test.db");
}
