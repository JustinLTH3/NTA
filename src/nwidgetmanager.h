#pragma once
#include <QObject>
#include <QPointer>
#include <spdlog/spdlog.h>

#include "nwidget.h"

namespace ads
{
    class CDockManager;
}

namespace NTA
{
    class NWidgetManager : public QObject
    {
        Q_OBJECT
        friend class NWidget;

    public:
        static QPointer<NWidgetManager> getInstance();
        static void destroyInstance();
        static bool createInstance(const QPointer<ads::CDockManager>& dockManager);
        template<typename T>
        T* createWidget(const QString& title, QWidget* parent = nullptr);
        QMap<QString, QList<QPointer<NWidget>>> widgets;
    signals:
        void currentFocusedNoteChanged(int64_t oldId, int64_t newId);

    protected:
        explicit NWidgetManager(const QPointer<ads::CDockManager>& dockManager, QObject* parent = nullptr);
        ~NWidgetManager() override;
        void removeWidget(NWidget* widget);
        static QPointer<NWidgetManager> instance;
        QPointer<ads::CDockManager> dockManager;
        int64_t id = -1;
    };

    template<typename T>
    T* NWidgetManager::createWidget(const QString& title, QWidget* parent)
    {
        NWidget* result = new T(dockManager, title, parent);
        result->typeName = typeid(T).name();
        widgets[result->typeName].append(result);
        spdlog::info(result->typeName.toStdString());
        return qobject_cast<T*>(result);
    }
} // NTA
