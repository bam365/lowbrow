#ifndef LBWEBVIEW_H
#define LBWEBVIEW_H

#include <QWebView>
#include "lbmain.h"



class LBWebView : public QWebView
{
Q_OBJECT

public:
        explicit LBWebView(LBMain *parent, KeyPressHandler kph);

signals:

public slots:

protected:
        void keyPressEvent(QKeyEvent*);


private:
        LBMain* m_lbm;
        KeyPressHandler m_kphandler;

};

#endif // LBWEBVIEW_H
