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

    public:
        explicit NWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent = nullptr);
        virtual ~NWidget() override;

        [[nodiscard]] QSharedPointer<Note> getNote() const;
        [[nodiscard]] bool getIsLinked() const;
        void linkNote(const QSharedPointer<Note>& inNote);

    protected:
        QSharedPointer<Note> note;
        bool isLinked = false;

    protected slots:
        void onFloat(bool isFloating);
        void togglePin();
        /**
         * @param newNote The new focusing note
         */
        virtual void onFocusNoteChanged(const QSharedPointer<Note>& newNote) = 0;

    private:
        Ui::NWidget* ui;
    };
} // NTA

#endif //NTA_NWIDGET_H
