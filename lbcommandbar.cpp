#include "lbcommandbar.h"
#include <QKeyEvent>


LBCommandBar::LBCommandBar(QWidget *parent) :
    QLineEdit(parent)
{
        connect(this, SIGNAL(returnPressed()),
                this, SLOT(retPressed()));
        hide();
}


void LBCommandBar::getarg(QString cmd)
{
        show(cmd);
}


void LBCommandBar::getcmd()
{
        show(QString(""));
}


void LBCommandBar::retPressed()
{
        QString cmd;

        cmd = text();
        hide();
        emit(cmdentered(cmd));
}



void LBCommandBar::show(QString begin)
{
        //TODO: Magic numbers
        setMinimumHeight(20);
        setMaximumHeight(20);
        setText(begin);
        setFocus(Qt::OtherFocusReason);
}


void LBCommandBar::hide()
{
        clearFocus();
        setText("");
        setMinimumHeight(0);
        setMaximumHeight(0);
}


void LBCommandBar::keyPressEvent(QKeyEvent *ev)
{
        if (ev->key() == Qt::Key_Escape)
                hide();
        else
                QLineEdit::keyPressEvent(ev);
}
