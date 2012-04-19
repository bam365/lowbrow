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
class LBCommandBar;
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

public: //Constants
        enum Modes {
                MODE_NORMAL,
                MODE_COMMAND,
                MODE_INPUT,
                MODE_PASSTHROUGH
        };

public: //Interface
        explicit LBMain(QWidget *parent = 0);
        ~LBMain();

        bool addbind(char* str, char* func, char *arg);

public:  //Commands
        void open(QString url);
        void tabopen(QString url);
        void back(QString arg);
        void scroll(QString arg);


public slots:
        void setProgress(int);
        void setUrl(const QUrl&);
        void loadStarted();
        void loadFinished(bool);
        void commandEntered(QString);

protected:
        bool kphandler(QKeyEvent*);
        void init();
        void init_cmdbar();
        void add_new_tab(QString& url);
        void connect_curr_tab_ss();
        void update_sb();
        QString get_arg(char* str);


private: //UI children
        Ui::LBMain* ui;
        LBCommandBar* m_cmdbar;

private: //Data
        std::vector<Tab> m_tabs;
        std::vector<Tab>::iterator m_currtab;
        Bindings m_bindings;
        int  m_mode;
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
