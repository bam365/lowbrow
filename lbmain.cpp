#include "lbmain.h"
#include "lbwebview.h"
#include "ui_lbmain.h"
#include <QStackedWidget>


Command* newcmd(LBMain* obj, void (LBMain::*f)(QString), char* arg)
{
        Command *cmd = new Command();

        cmd->f = f;
        cmd->obj = obj;
        cmd->arg = new QString(arg);

        return cmd;
}


LBMain::LBMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LBMain)
{
        ui->setupUi(this);
        init();
}


void LBMain::init()
{
        QString homepage;

        ui->ledCmd->setMaximumHeight(0);
        ui->lblTabs->setMaximumHeight(0);
        ui->lblTabs->setMinimumHeight(0);
        homepage = "http://www.google.com";
        add_new_tab(homepage);
}


void LBMain::add_new_tab(QString& url)
{
        Tab nt;
        QStackedWidget *sw = ui->stkWp;
        int i;

        nt.title = new QString("Test");
        nt.url = new QUrl(url, QUrl::TolerantMode);
        nt.wv = new LBWebView();
        nt.wv->load(*(nt.url));
        //nt.wv->show();
        m_tabs.push_back(nt);
        m_currtab = m_tabs.end() - 1;
        m_currtab->wv->show();
        i = sw->addWidget(m_currtab->wv);
        sw->setCurrentIndex(i);
        sw->show();
}


LBMain::~LBMain()
{
        delete ui;
}
