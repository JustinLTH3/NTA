//
// Created by JustinLai on 12/3/2026.
//

#include "nwidgetmanagertest.h"

#include "NTestWidget.h"
#include "../src/space.h"
#include "../src/nwidgetmanager.h"

void NWidgetManagerTest::initTestCase()
{
    QVERIFY(NTA::NWidgetManager::getInstance() == nullptr);
    m_DockManager = new ads::CDockManager();
    space.reset(NTA::Space::createSpace(QDir::current(), "space.nta"));
    QVERIFY(space != nullptr);
    NTA::NNoteManager::createInstance(space);
}

void NWidgetManagerTest::init()
{
}

void NWidgetManagerTest::createInstanceTest()
{
    QVERIFY(NTA::NWidgetManager::createInstance(m_DockManager));
    QVERIFY(NTA::NWidgetManager::getInstance());
}

void NWidgetManagerTest::destroyInstanceTest()
{
    QVERIFY(NTA::NWidgetManager::createInstance(m_DockManager));
    NTA::NWidgetManager::destroyInstance();
    QVERIFY(NTA::NWidgetManager::getInstance() == nullptr);
}

void NWidgetManagerTest::createInstanceWithNullDockManagerTest()
{
    QVERIFY(NTA::NWidgetManager::createInstance(nullptr) == false);
}

void NWidgetManagerTest::createInstanceWhenAlreadyExistTest()
{
    QVERIFY(NTA::NWidgetManager::createInstance(m_DockManager));
    QVERIFY(NTA::NWidgetManager::createInstance(m_DockManager) == false);
}

void NWidgetManagerTest::createWidgetTest()
{
    NTA::NWidgetManager::createInstance(m_DockManager);
    auto testWidget = NTA::NWidgetManager::getInstance()->createWidget<NTestWidget>("Test Widget");
    QVERIFY(testWidget != nullptr);
    QVERIFY(testWidget->dockManager() == m_DockManager);
    QVERIFY(testWidget->parent() == nullptr);
    QVERIFY(testWidget->typeName == typeid(NTestWidget).name());
    QVERIFY(NTA::NWidgetManager::getInstance()->widgets[testWidget->typeName].size()==1);
    QVERIFY(NTA::NWidgetManager::getInstance()->widgets[testWidget->typeName][0] == testWidget);
    testWidget->deleteLater();
}

void NWidgetManagerTest::removeWidgetTest()
{
    NTA::NWidgetManager::createInstance(m_DockManager);
    QPointer<NTestWidget> testWidget = NTA::NWidgetManager::getInstance()->createWidget<NTestWidget>("Test Widget");
    QVERIFY(NTA::NWidgetManager::getInstance()->widgets[testWidget->typeName].size()==1);
    NTA::NWidgetManager::getInstance()->removeWidget(testWidget);
    QVERIFY(NTA::NWidgetManager::getInstance()->widgets[testWidget->typeName].size()==0);
    QTest::qWait(200);
    QVERIFY(testWidget.isNull());
}

void NWidgetManagerTest::cleanup()
{
    NTA::NWidgetManager::destroyInstance();
}

void NWidgetManagerTest::cleanupTestCase()
{
    delete m_DockManager;
    space.reset();
    NTA::NNoteManager::destroyInstance();
    if (QDir::current().exists("space.nta"))
        (QDir::current().remove("space.nta"));
}
