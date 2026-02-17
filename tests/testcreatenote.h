#pragma once
#include <QTest>

namespace NTA
{
    class Space;
}

class TestCreateNote : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void createDefaultNoteTest();
    void createNoteWithTitleTest();
    void createNoteWithOtherTypeIdTest();
    void createNoteWithInvalidTypeIdTest();
    void cleanup();

private:
    QSharedPointer<NTA::Space> space;
};
