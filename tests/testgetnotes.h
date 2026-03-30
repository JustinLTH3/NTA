#pragma once
#include "QTest"


namespace NTA
{
    class Space;
}

class TestGetNotes : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void getNoteWithIdTest();
    void getNonExistingNoteTest();
    void searchNoteByTextTest();
    void searchNoteBySpecificTextTest();
    void searchNoteByEmptyTextTest();
    void searchNoteWithSpecificColumnTest();
    void cleanup();

private:
    QSharedPointer<NTA::Space> space;
};
