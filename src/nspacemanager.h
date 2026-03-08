#pragma once
#include <QObject>
#include <QSharedPointer>


namespace NTA
{
    class Space;

    class NSpaceManager : public QObject
    {
        Q_OBJECT

    public:
        static QPointer<NSpaceManager> getInstance();
        static void destroyInstance();
        static bool createInstance(const QSharedPointer<Space>& in_space);

    protected:
        explicit NSpaceManager(const QSharedPointer<Space>& in_space);
        QSharedPointer<Space> space;
        static QPointer<NSpaceManager> instance;
    };
}
