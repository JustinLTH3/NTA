#ifndef NTA_NEDITORWIDGET_H
#define NTA_NEDITORWIDGET_H

#include <QLineEdit>
#include <QTextEdit>
#include <QTimer>

#include "nwidget.h"

class QPlainTextEdit;

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
        void onNoteUpdated(int64_t id, NWidget* from, unsigned int columns) override;

    protected:
        QPlainTextEdit* content = nullptr;
        QLineEdit* titleBar = nullptr;
        QTimer saveTimer;
    };
} // NTA

#endif //NTA_NEDITORWIDGET_H
