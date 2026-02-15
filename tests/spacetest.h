#pragma once
#include "QTest"

class SpaceTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void createSpaceTest();
    void createSpaceWhenSpaceExistTest();
    void createSpaceWhenEmptyFileNameTest();
    void createSpaceWhenDirNotExistTest();
    void cleanup();

private:
    QDir dir;
};
