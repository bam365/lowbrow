#include "lbmain.h"
#include "bindings.h"
#include "lbwebview.h"
#include "lbcommandbar.h"
#include "ui_lbmain.h"
#include <QStackedWidget>
#include <QKeyEvent>
#include <QWebFrame>

using namespace std;


//Commands
static struct CmdMap {
        char *str;
        LBCmdFunc fn;
} CMAP[] = {
        {"open", &LBMain::open},
        {"back", &LBMain::back},
        {"tabopen", &LBMain::tabopen},
        {"scroll", &LBMain::scroll},
};


//TODO: Ok, I need to reevaluate my life here. These next two functions are set
// up to search this CMAP in logarithmic time in terms of the string value.
// However, searching it by function, which happens every time the user uses
// a command without a required argument, is in linear time. That's probably
// the reverse of what it should be.

//Recursive quicksort of command map, so we can binary search it later
void sort_cmap(struct CmdMap *beg, struct CmdMap *end)
{
        struct CmdMap tmp;
        struct CmdMap *i = beg;
        struct CmdMap *j = end;
        char *pivot = (i + (j-i)/2)->str;

        while (i <= j) {
                for (; strcmp(i->str, pivot) < 0; i++);
                for (; strcmp(j->str, pivot) > 0; j--);
                if (i <= j) {
                        memcpy(&tmp, i, sizeof(tmp));
                        memcpy(i, j, sizeof(*i));
                        memcpy(j, &tmp, sizeof(*j));
                        i++, j--;
                }
        }
        if (beg < j)
                sort_cmap(beg, j);
        if (i < end)
                sort_cmap(i, end);
}


//Recursive binary search of cmap, returns fn if found or NULL if not found
//TODO: This still doesn't work right
LBCmdFunc find_cmap(char *str, struct CmdMap *beg, struct CmdMap *end)
{
        struct CmdMap *foo = CMAP;
        if (!beg || !end || !str)
                return NULL;
        if (end == beg) {
                if (!strcmp(beg->str, str))
                        return beg->fn;
                else
                        return NULL;
        }

        struct CmdMap *mid = (beg + (end-beg)/2);
        int cmp = strcmp(str, mid->str);
        if (cmp < 0)
                return find_cmap(str, beg, mid-1);
        else if (cmp > 0)
                return find_cmap(str, mid+1, end);
        else
                return mid->fn;
}


char *find_cmap(LBCmdFunc fn, struct CmdMap *beg, struct CmdMap *end)
{
        struct CmdMap *c;

        for (c = beg; c <= end; c++)
                if (c->fn == fn)
                        return c->str;
        return NULL;
}



Command* newcmd(LBMain* obj, LBCmdFunc f, char* arg)
{
        Command *cmd = new Command();

        cmd->f = f;
        cmd->obj = obj;
        cmd->arg = new QString(arg);

        return cmd;
}


LBMain::LBMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LBMain),
    m_cmdbar(NULL)
{
        ui->setupUi(this);
        init();
}


//TODO: There's a lot of magic constants here and throughout that need to be
//      replaced with settings
void LBMain::init()
{
        QString homepage;

        sort_cmap(CMAP, CMAP+ARRYSIZE(CMAP)-1);

        init_cmdbar();
        ui->lblTabs->setMaximumHeight(0);
        ui->lblTabs->setMinimumHeight(0);
        homepage = "http://www.google.com";
        m_mode = MODE_NORMAL;
        add_new_tab(homepage);
        update_sb();
        m_currtab->wv->setFocus(Qt::OtherFocusReason);
}


void LBMain::init_cmdbar()
{
        if (!m_cmdbar)
                m_cmdbar = new LBCommandBar(this);
        connect(m_cmdbar, SIGNAL(cmdentered(QString)),
                this, SLOT(commandEntered(QString)));
        ui->vloMain->addWidget(m_cmdbar);
}


bool LBMain::kphandler(QKeyEvent* key)
{
        int k = key->key();
        int mods = key->modifiers();
        Command *cmd = NULL;

        if (key->modifiers() == Qt::ControlModifier && key->key() == 'P')
                m_mode = (m_mode == MODE_PASSTHROUGH) ?
                                        MODE_NORMAL : MODE_PASSTHROUGH;
        if (m_mode == MODE_PASSTHROUGH)
                return false;

        if (k == Qt::Key_Escape) {
                m_cmd = "";
                update_sb();
                return true;
        } else if (mods) { cmd = m_bindings.findmod(mods, k);
        } else {
                m_cmd += key->text().toAscii().data();
                cmd = m_bindings.find(m_cmd.c_str());
        }
        if (cmd) {
                m_cmd = "";
                cmd->exec();
                update_sb();
        } else if (!m_bindings.find_startswith(m_cmd.c_str())) {
                ui->lblCmd->setText(QString("Bad cmd \"%1\"")
                                    .arg(m_cmd.c_str()));
                m_cmd = "";
        } else {
                ui->lblCmd->setText(m_cmd.c_str());
        }
        return true;
}


void LBMain::add_new_tab(QString& url)
{
        Tab nt;
        QStackedWidget *sw = ui->stkWp;
        int i;

        nt.title = new QString("Test");
        nt.url = new QUrl(url, QUrl::TolerantMode);
        nt.wv = new LBWebView(this, &LBMain::kphandler);
        nt.wv->load(*(nt.url));
        m_tabs.push_back(nt);
        m_currtab = m_tabs.end() - 1;
        connect_curr_tab_ss();
        m_currtab->wv->show();
        i = sw->addWidget(m_currtab->wv);
        sw->setCurrentIndex(i);
        sw->show();
}


void LBMain::connect_curr_tab_ss()
{
        LBWebView* wv = m_currtab->wv;

        connect (wv, SIGNAL(urlChanged(const QUrl&)),
                 this, SLOT(setUrl(const QUrl&)));
        connect (wv, SIGNAL(loadStarted()),
                 this, SLOT(loadStarted()));
        connect (wv, SIGNAL(loadProgress(int)),
                 this, SLOT(setProgress(int)));
        connect (wv, SIGNAL(loadFinished(bool)),
                 this, SLOT(loadFinished(bool)));
}


void LBMain::update_sb()
{
        QString url;

        ui->lblCmd->setText(m_cmd.c_str());
        ui->lblPerc->setText("");
}


bool LBMain::addbind(char* str, char* lbf, char *arg)
{
        LBCmdFunc cmdfn;

        cmdfn = find_cmap(lbf, CMAP, CMAP+ARRYSIZE(CMAP)-1);
        if (!cmdfn)
                return false;
        else
                return m_bindings.add(str, newcmd(this, cmdfn, arg));
}



void LBMain::open(QString url)
{
        if (url == "") {
                m_cmdbar->getarg(find_cmap(&LBMain::open, CMAP,
                                 CMAP + ARRYSIZE(CMAP)-1));
        } else {
                m_currtab->wv->load(QUrl(url, QUrl::TolerantMode));
        }
}


void LBMain::tabopen(QString url)
{
        if (url == "")
                m_cmdbar->getarg(find_cmap(&LBMain::tabopen, CMAP,
                                 CMAP + ARRYSIZE(CMAP)-1));
        else
                add_new_tab(url);
}


void LBMain::scroll(QString arg)
{
        QWebFrame* frm = m_currtab->wv->page()->mainFrame();

        bool ok;
        int dy = arg.toInt(&ok);
        if (!ok)
                dy = 5;
        int cp = frm->scrollBarValue(Qt::Vertical);
        frm->setScrollBarValue(Qt::Vertical, cp+dy);
}


void LBMain::back(QString arg)
{
        m_currtab->wv->back();
}

void LBMain::setUrl(const QUrl& url)
{
        ui->lblUrl->setText(url.toEncoded());
}

void LBMain::setProgress(int p)
{
        QString pstr;

        ui->lblProgress->setText(QString("Loading...%1%").arg(p));
}

void LBMain::loadStarted()
{
        ui->lblProgress->setText("Loading...0%");
}

void LBMain::loadFinished(bool)
{
        ui->lblProgress->setText("Loaded.");
}


void LBMain::commandEntered(QString txt)
{
        QString cmd, arg;
        QStringList sl;
        LBCmdFunc fn = NULL;

        m_currtab->wv->setFocus(Qt::OtherFocusReason);
        sl = txt.split(' ', QString::SkipEmptyParts);
        if (sl.count() < 1)
                return;
        cmd = sl[0];
        if (sl.count() > 1) {
                sl.pop_front();
                arg = sl.join(" ");
        }
        fn = find_cmap(cmd.toAscii().data(), CMAP, CMAP+ARRYSIZE(CMAP)-1);
        if (fn) {
                (this->*fn)(arg);
        }
}


LBMain::~LBMain()
{
        delete ui;
        if (m_cmdbar) {
                delete m_cmdbar;
                m_cmdbar = NULL;
        }
}
