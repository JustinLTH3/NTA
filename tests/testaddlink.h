#pragma once
#include <QTest>


namespace NTA
{
    class Space;
}

class TestAddLink : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void addLinkTest();
    void addAlreadyExistingLinkTest();
    void addLinkToNonExistingNoteTest();
    void addReverseLinkTest();
    void addLinkToSelfTest();
    void addMultipleLinksFromSameSourceTest();
    void addMultipleLinksToSameTargetTest();
    void cleanup();

private:
    QSharedPointer<NTA::Space> space;
};
