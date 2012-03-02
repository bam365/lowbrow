#ifndef LBWEBVIEW_H
#define LBWEBVIEW_H

#include <QWebView>
#include "lbmain.h"



class LBWebView : public QWebView
{
Q_OBJECT

public:
        explicit LBWebView(QWidget *parent = 0);

        void setKeybindings(Keybindings*);

        //Properties
        bool passThrough;

signals:

public slots:

protected:
        void keyPressEvent(QKeyEvent *ev);

private:
        Keybindings* m_kb;

};

#endif // LBWEBVIEW_H
