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
    void initTestCase();
    void openExistingSpaceTest();
    void openNonExistingSpaceTest();
    void cleanup();
    void cleanupTestCase();

private:
    QDir dir;
};
