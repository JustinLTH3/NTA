#pragma once
#include <QTest>


namespace NTA
{
    class Space;
}

class NoteManagerTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void createInstanceTest();
    void destroyInstanceTest();
    void createInstanceWithNullSpaceTest();
    void createInstanceWhenAlreadyExistTest();
    void cleanupTestCase();

private:
    QSharedPointer<NTA::Space> space;
};
