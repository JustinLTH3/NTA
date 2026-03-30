#ifndef NTA_NWIDGET_H
#define NTA_NWIDGET_H

#include "DockWidget.h"

namespace NTA
{
    struct Note;
    QT_BEGIN_NAMESPACE

    namespace Ui
    {
        class NWidget;
    }

    QT_END_NAMESPACE

    class NWidget : public ads::CDockWidget
    {
        Q_OBJECT
        friend class NWidgetManager;

    public:
        explicit NWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent = nullptr);
        virtual ~NWidget() override;

        [[nodiscard]] QSharedPointer<Note> getNote() const;
        [[nodiscard]] bool getIsLinked() const;
        virtual void linkNote(const QSharedPointer<Note>& inNote, bool linked = true);
        virtual void setNote(const QSharedPointer<Note>& inNote);
        void linkCurrent(bool l = true);

    protected:
        QSharedPointer<Note> note;
        bool isLinked = false;
        QString typeName;
        QAction* linkCurrentAction;

    protected slots:
        void onFloat(bool isFloating);
        void togglePin();
        virtual void onFocusNoteChanged(int64_t old, int64_t now) = 0;

        virtual void onNoteUpdated(int64_t id, NWidget* from, unsigned int columns)
        {
        }

    private:
        Ui::NWidget* ui;
    };
} // NTA

#endif //NTA_NWIDGET_H
