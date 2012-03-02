#include <QtGui/QApplication>
#include "lbmain.h"

#define SHIFTKEY Qt::ShiftModifier
#define ALTKEY   Qt::AltModifier


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Keybindings kb;
    LBMain w;

    kb['o'] = newcmd(&w, &LBMain::open, "www.google.com");
    w.show();
    
    return a.exec();
}
