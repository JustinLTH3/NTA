//
// Created by JustinLai on 22/3/2026.
//

#include "nrenderwidget.h"

#include <QPointer>
#include <QScrollBar>
#include <qtextbrowser.h>
#include <QTextEdit>
#include <QVBoxLayout>

#include "md4c-html.h"
#include "nmd.h"
#include "note.h"
#include "nspacemanager.h"

namespace NTA
{
    static void captureHtmlFragment(const MD_CHAR* data, MD_SIZE data_size, void* userData)
    {
        QByteArray* array = static_cast<QByteArray*>(userData);

        if (data_size > 0)
        {
            array->append(data, int(data_size));
        }
    }


    NRenderWidget::NRenderWidget(ads::CDockManager* dockManager, const QString& title, QWidget* parent)
        : NWidget(dockManager, title, parent)
    {
        static QWeakPointer<nmd> mdP;
        if (mdP.isNull())
        {
            mdParser = QSharedPointer<nmd>(new nmd);
            mdP = mdParser;
        }
        else
        {
            mdParser = mdP.toStrongRef();
        }
        auto central = new QWidget(this);
        setWidget(central);
        central->setLayout(new QVBoxLayout(central));
        textBrowser = new QTextBrowser();
        central->layout()->addWidget(textBrowser);
        // QString text = "# Testing html\n"
        //     "[Datatypes In SQLite](https://www.sqlite.org/datatype3.html) [[Wiki link]]Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus ullamcorper urna non quam pretium, maximus ornare orci maximus. Donec et condimentum tellus. Donec gravida sodales pellentesque. Donec pharetra nisi tortor, sit amet ultrices nibh luctus id. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. In dolor elit, scelerisque vitae dolor eu, hendrerit pellentesque ante. Etiam massa tellus, tristique ut diam et, aliquet pretium arcu. Morbi placerat augue sit amet sapien commodo, non posuere dui bibendum. Sed sodales lorem sed erat hendrerit, a pellentesque enim imperdiet. In lorem lacus, sagittis eget nulla nec, ultrices sollicitudin mauris. Sed pulvinar congue ante, nec tempor nulla elementum vitae. Sed porttitor ipsum sit amet lectus fringilla ultrices. Phasellus tristique arcu nunc, hendrerit tristique nunc tristique sodales. Phasellus euismod, urna vitae sodales blandit, tortor erat congue sapien, quis porttitor arcu lorem eu erat. Mauris ut urna ut dolor scelerisque iaculis ac in diam."
        //     "\n ``` c++ \n int main(); \n```\n"
        //     "abcdefghijklmnopqrstuvwxyz\n"
        //     "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
        //     "[[abcdefghijklmnopqrstuvwxyz | abcdefghijklmnopqrstuvwxyz]]\n"
        //     "[[ABCDEFGHIJKLMNOPQRSTUVWXYZ | ABCDEFGHIJKLMNOPQRSTUVWXYZ]]";
        // MD4C flags
    }

    void NRenderWidget::setNote(const QSharedPointer<Note>& inNote)
    {
        NWidget::setNote(inNote);
        if (inNote)
        {
            if (QString s; !mdParser->md2html(inNote->body, s))
                textBrowser->setHtml(s);
        }
    }

    void NRenderWidget::onFocusNoteChanged(int64_t old, int64_t now)
    {
        if (isLinked || old == now)return;
        setNote(NNoteManager::getInstance()->getNoteWithId(now));
    }

    void NRenderWidget::onNoteUpdated(int64_t id, NWidget* from, unsigned int columns)
    {
        if (id != note->id)return;
        if (columns & NoteColumn::body)
        {
            if (QString s; !mdParser->md2html(note->body, s))
            {
                int v = textBrowser->verticalScrollBar()->value();
                textBrowser->setHtml(s);
                textBrowser->verticalScrollBar()->setValue(v);
            }
        }
    }
} // NTA
