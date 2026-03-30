//
// Created by JustinLai on 12/3/2026.
//

#pragma once
#include "DockManager.h"
#include "QTest"

namespace NTA
{
    class Space;
}

class NWidgetManagerTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void createInstanceTest();
    void destroyInstanceTest();
    void createInstanceWithNullDockManagerTest();
    void createInstanceWhenAlreadyExistTest();
    void createWidgetTest();
    void removeWidgetTest();
    void cleanup();
    void cleanupTestCase();

protected:
    ads::CDockManager* m_DockManager = nullptr;
    QSharedPointer<NTA::Space> space = nullptr;
};
