//
// Created by JustinLai on 15/3/2026.
//

#pragma once
#include <QObject>
#include <QSharedPointer>
#include <SQLiteCpp/Statement.h>

namespace NTA
{
    class Space;

    class NLinkManager : public QObject
    {
        Q_OBJECT

    public:
        static QPointer<NLinkManager> getInstance();
        static void destroyInstance();
        static bool createInstance(const QSharedPointer<Space>& in_space);

        /**
         * @param from The source id of the link.
         * @param to The target id of the link.
         * @return Success or not.
         */
        bool addLink(int64_t from, int64_t to);
        bool removeLink(int64_t from, int64_t to);
        bool editLink(int64_t from, int64_t to, QString description, QString alias);
        SQLite::Statement getLinks(int64_t from);
        SQLite::Statement getBackLinks(int64_t to);
        SQLite::Statement searchLinks(QString param);
        SQLite::Statement searchLinksOfSource(QString param, int64_t sourceId);

    protected:
        explicit NLinkManager(const QSharedPointer<Space>& in_space);
        QSharedPointer<Space> space;
        static QPointer<NLinkManager> instance;
    };
} // NTA
