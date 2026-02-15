#pragma once
#include "QTest"

namespace NTA
{
    class Space;
}

class OpenSpaceTest : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void openExistingSpaceTest();
    void openNonExistingSpaceTest();
    void openNullSpaceTest();
    void openDatabaseWithWrongHeaderTest();
    void cleanup();
    void cleanupTestCase();

private:
    QDir dir;
};
