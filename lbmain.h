#ifndef LBMAIN_H
#define LBMAIN_H

#include <QMainWindow>
#include <QUrl>
#include <QString>
#include <vector>
#include <map>
#include "bindings.h"

//Forward decl
class LBWebView;
class Command;
class LBMain;
class Bindings;

namespace Ui {
        class LBMain;
}


typedef bool (LBMain::*KeyPressHandler)(QKeyEvent*);



struct Tab {
        QString* title;
        QUrl* url;
        LBWebView* wv;
};



class LBMain : public QMainWindow
{
Q_OBJECT

public:
        explicit LBMain(QWidget *parent = 0);

        void addbind(char* str, char* func, char *arg);

        //Commands
        void open(QString url);
        void tabopen(QString url);
        void scrolldown(QString arg);
        void scrollup(QString arg);

        ~LBMain();

protected:
        bool kphandler(QKeyEvent*);
        void init();
        void add_new_tab(QString& url);


private:
        Ui::LBMain *ui;

        std::vector<Tab> m_tabs;
        std::vector<Tab>::iterator m_currtab;
        Bindings m_bindings;
        bool m_passthrough;
};




struct Command {
        void (LBMain::*f)(QString);
        LBMain* obj;
        QString* arg;

        void exec()
        {
                if (f && obj) {
                        if (arg)
                                (obj->*f)(*arg);
                        else
                                (obj->*f)(QString(""));
                }
        }
};

Command* newcmd(LBMain* obj, void (LBMain::*f)(QString), char* arg);


#endif // LBMAIN_H
