#ifndef LBMAIN_H
#define LBMAIN_H

#include <QMainWindow>
#include <QUrl>
#include <QString>
#include <vector>
#include <map>

class LBWebView;


namespace Ui {
        class LBMain;
}


struct Tab {
        QString* title;
        QUrl* url;
        LBWebView* wv;
};


class Command;
typedef std::map<char*, Command*> Keybindings;



class LBMain : public QMainWindow
{
Q_OBJECT

public:
        explicit LBMain(QWidget *parent = 0);

        void setKeybindings(Keybindings kb);

        void open(QString url);
        void tabopen(QString url);

        ~LBMain();

protected:
        void init();
        void add_new_tab(QString& url);


private:
        Ui::LBMain *ui;

        std::vector<Tab> m_tabs;
        std::vector<Tab>::iterator m_currtab;
};



struct Command {
        void (LBMain::*f)(QString);
        LBMain* obj;
        QString* arg;

        void exec()
        {
                if (f && obj)
                        if (arg)
                                (obj->*f)(*arg);
                        else
                                (obj->*f)(QString(""));
        }
};

Command* newcmd(LBMain* obj, void (LBMain::*f)(QString), char* arg);


#endif // LBMAIN_H
