#pragma once
#include <qlistwidget.h>

#include "nwidget.h"

namespace NTA
{
    class NBacklinkWidget : public NWidget
    {
        Q_OBJECT

    public:
        explicit NBacklinkWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent);
        ~NBacklinkWidget() override;
        void linkNote(const QSharedPointer<Note>& inNote, bool linked) override;
        void setNote(const QSharedPointer<Note>& inNote) override;

    protected slots:
        void onFocusNoteChanged(int64_t old, int64_t now) override;
        void onNoteUpdated(int64_t id, NWidget* from, unsigned int columns) override;

    private:
        QListWidget* listWidget;
    };
} // NTA
