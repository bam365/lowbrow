#ifndef LBMAIN_H
#define LBMAIN_H

#include <QMainWindow>
#include <QUrl>
#include <QString>
#include <vector>
#include <string>
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

#define ARRYSIZE(x) (sizeof(x) / sizeof((x)[0]))


typedef bool (LBMain::*KeyPressHandler)(QKeyEvent*);
typedef void (LBMain::*LBCmdFunc)(QString);



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

        bool addbind(char* str, char* func, char *arg);

        //Commands
        void open(QString url);
        void tabopen(QString url);
        void back(QString arg);
        void scroll(QString arg);

        ~LBMain();

public slots:
        void setProgress(int);
        void setUrl(const QUrl&);
        void loadStarted();
        void loadFinished(bool);
        void commandEntered();

protected:
        bool kphandler(QKeyEvent*);
        void init();
        void add_new_tab(QString& url);
        void connect_curr_tab_ss();
        void update_sb();
        QString get_arg(char* str);


private:
        Ui::LBMain *ui;

        std::vector<Tab> m_tabs;
        std::vector<Tab>::iterator m_currtab;
        Bindings m_bindings;
        bool m_passthrough;
        std::string m_cmd;
};




struct Command {
        LBCmdFunc f;
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


Command* newcmd(LBMain* obj, LBCmdFunc f, char* arg);


#endif // LBMAIN_H
