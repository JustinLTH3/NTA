#pragma once
#include <QListWidget>

#include "nwidget.h"

namespace NTA
{
    class NLinkWidget : public NWidget
    {
        Q_OBJECT

    public:
        explicit NLinkWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent = nullptr);
        ~NLinkWidget() override;
        void linkNote(const QSharedPointer<Note>& inNote, bool linked) override;
        void setNote(const QSharedPointer<Note>& inNote) override;

    protected slots:
        void onFocusNoteChanged(int64_t old, int64_t now) override;
        void onNoteUpdated(int64_t id, NWidget* from, unsigned int columns) override;

    protected:
        QListWidget* listWidget = nullptr;
    };
} // NTA
