#include <QKeyEvent>
#include "lbwebview.h"
#include "lbmain.h"


LBWebView::LBWebView(QWidget *parent) :
    QWebView(parent)
{
        m_kb = NULL;
        passThrough = false;
}


void LBWebView::setKeybindings(Keybindings* kb)
{
        m_kb = kb;
}


void LBWebView::keyPressEvent(QKeyEvent *ev)
{
        int key = static_cast<int>(ev->text()[0]);
        Keybindings::iterator i;
        Command* cmd = NULL;


        if (passThrough) {
                QWebView::keyPressEvent(ev);
        } else {
                if ((i = m_kb->find(key)) != m_kb->end())
                        cmd = i->second;
                if (cmd)
                        cmd->exec();
                else
                        QWebView::keyPressEvent(ev);
        }

}
