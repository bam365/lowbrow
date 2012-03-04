#include <QtGui/QApplication>
#include "lbmain.h"
#include "bindings.h"

#define SHIFTKEY Qt::ShiftModifier
#define ALTKEY   Qt::AltModifier


void set_default_bindings(LBMain* lbm);

int main(int argc, char *argv[])
{
        QApplication a(argc, argv);
        Bindings bindings;
        LBMain* w = new LBMain();
        int ret;

        set_default_bindings(w);

        w->show();
        ret = a.exec();
        delete w;
        return ret;
}


void set_default_bindings(LBMain* lbm)
{
        lbm->addbind("j", "scroll", "20");
        lbm->addbind("k", "scroll", "-20");
        lbm->addbind("b", "back", NULL);
        lbm->addbind("o", "open", NULL);
        lbm->addbind("goc", "open", "http://www.cnn.com");
        lbm->addbind("gof", "open", "http://www.foxnews.com");
        lbm->addbind("gog", "open", "http://www.google.com");
}
