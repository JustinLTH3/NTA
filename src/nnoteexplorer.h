//
// Created by JustinLai on 8/3/2026.
//

#ifndef NTA_NNOTEEXPLORER_H
#define NTA_NNOTEEXPLORER_H

#include "nwidget.h"

class QListWidgetItem;
class QListWidget;

namespace NTA
{
    class NNoteExplorer : public NWidget
    {
        Q_OBJECT

    public:
        explicit NNoteExplorer(ads::CDockManager* dockManager, const QString& title, QWidget* parent = nullptr);
        ~NNoteExplorer() override;

        void linkNote(const QSharedPointer<Note>& inNote, bool linked) override;
        void setNote(const QSharedPointer<Note>& inNote) override;

    protected slots:
        void onFocusNoteChanged(int64_t old, int64_t now) override;

    protected:
        QListWidget* listWidget = nullptr;
        QMap<QListWidgetItem*, int64_t> items;
        QMap<int64_t, QListWidgetItem*> ids;
    };
} // NTA

#endif //NTA_NNOTEEXPLORER_H
