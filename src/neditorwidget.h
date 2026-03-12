#ifndef NTA_NEDITORWIDGET_H
#define NTA_NEDITORWIDGET_H

#include "nwidget.h"

namespace NTA
{
    class NEditorWidget : public NWidget
    {
        Q_OBJECT

    public:
        explicit NEditorWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent = nullptr);
        ~NEditorWidget() override;

        void linkNote(const QSharedPointer<Note>& inNote, bool linked) override;
        void setNote(const QSharedPointer<Note>& inNote) override;
    protected slots:
        void onFocusNoteChanged(int64_t old, int64_t now) override;
    };
} // NTA

#endif //NTA_NEDITORWIDGET_H
