#ifndef LBCOMMANDBAR_H
#define LBCOMMANDBAR_H

#include <QLineEdit>

//Forward decl
class QKeyEvent;


class LBCommandBar : public QLineEdit
{
        Q_OBJECT

public:
        explicit LBCommandBar(QWidget *parent = 0);

signals:
        void cmdentered(QString cmd);


public slots:
        void getarg(QString cmd);
        void getcmd();

protected slots:
        void retPressed();


protected: //Overrides
        void keyPressEvent(QKeyEvent*);

protected: //Utilities
        void show(QString);
        void hide();

};

#endif // LBCOMMANDBAR_H
