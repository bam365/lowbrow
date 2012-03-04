#include <QKeyEvent>
#include "lbwebview.h"


LBWebView::LBWebView(LBMain *parent, KeyPressHandler kph) :
    QWebView(parent)
{
        m_lbm = parent;
        m_kphandler = kph;
}


void LBWebView::keyPressEvent(QKeyEvent *ev)
{
        if (!(m_lbm->*m_kphandler)(ev))
                QWebView::keyPressEvent(ev);
}
