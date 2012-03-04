#include <QtGui/QApplication>
#include "lbmain.h"
#include "bindings.h"

#define SHIFTKEY Qt::ShiftModifier
#define ALTKEY   Qt::AltModifier


int main(int argc, char *argv[])
{
        QApplication a(argc, argv);
        Bindings bindings;
        LBMain* w = new LBMain();
        int ret;


        w->show();
        ret = a.exec();
        delete w;
        return ret;
}


void set_default_bindings(LBMain* lbm)
{
        bindings->add("goc", newcmd(w, &LBMain::open, "www.cnn.com"));
}
