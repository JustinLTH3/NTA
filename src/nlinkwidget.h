#pragma once
#include <QListWidget>

#include "nwidget.h"

class QToolButton;

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
        QToolButton* addBtn = nullptr;
        QToolButton* removeBtn = nullptr;
        QMap<QListWidgetItem*, int64_t> itemMap;
        void createItem(int64_t id, const QString& titleText, const QString& descriptionText, const QString& aliasText);
    };
} // NTA
