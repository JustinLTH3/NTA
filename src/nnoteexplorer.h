//
// Created by JustinLai on 8/3/2026.
//

#ifndef NTA_NNOTEEXPLORER_H
#define NTA_NNOTEEXPLORER_H

#include "nwidget.h"

namespace NTA
{
    class NNoteExplorer : public NWidget
    {
        Q_OBJECT

    public:
        explicit NNoteExplorer(ads::CDockManager* dockManager, const QString& title, QWidget* parent = nullptr);
        ~NNoteExplorer() override;

    protected slots:
        void onFocusNoteChanged(const QSharedPointer<Note>& newNote) override;
    };
} // NTA

#endif //NTA_NNOTEEXPLORER_H
