#pragma once
#include <QTest>


namespace NTA
{
    class Space;
}

class LinkManagerTest : public QObject
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
    void getLinksTest();
    void removeLinkTest();
    void removeNonExistingLinkTest();
    void searchLinkTest();
    void searchLinkFromSourceTest();
    void searchLinkWithEmptyParamsTest();
    void cleanup();

private:
    QSharedPointer<NTA::Space> space;
};
