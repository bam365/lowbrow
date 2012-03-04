#include "bindings.h"
#include "lbmain.h"


using namespace std;


//post: str is modified in-place
void remove_spaces(char *str)
{
        char* c, *r;

        for (c = str; *c != '\0'; c++)
                if (*c == ' ' || *c  == '\t')
                        for (r = c; *r != '\0'; r++)
                                *r = *(r+1);
}


Bindings::Bindings()
{
}


//post: str will have all spaces removed
bool Bindings::add(char* str, Command* cmd)
{
        int mk;

        if (!str || !cmd)
                return false;

        remove_spaces(str);
        if (is_mod_binding(str)) {
                if ((mk = modkey(str) == -1))
                        return false;
                m_modcmds[mk] = cmd;
        } else {
                m_cmds[string(str)] = cmd;
        }

        return true;
}


Command* Bindings::find(char* str)
{
        int mk;
        map<int, Command*>::iterator mci;
        map<string, Command*>::iterator ci;

        if (is_mod_binding(str)) {
                if ((mk = modkey(str) == -1))
                        return NULL;
                if ((mci = m_modcmds.find(mk)) == m_modcmds.end())
                        return NULL;
                else
                        return mci->second;
        } else {
                if ((ci = m_cmds.find(string(str))) == m_cmds.end())
                        return NULL;
                else
                        return ci->second;

        }

        return NULL;
}


Command* Bindings::findmod(short c, short mods)
{
        return NULL;
}


bool Bindings::is_mod_binding(char* str)
{
        int l;

        l = strlen(str);
        if (l < 5)
                return false;
        return (str[0] != '<' || str[l-1] != '>');
}


//pre: str should have spaces removed prior to this, and we should already
//     know that str is in the correct format
int Bindings::modkey(char* str)
{
        int mods, key;
        char *c;
        bool dashfound;

        if (!str)
                return -1;

        mods = key = 0;
        dashfound = false;
        for (c = str; *c != '>'; c++) {
                if (!dashfound) {
                        switch (*c) {
                        case 'C':
                                mods |= Qt::ControlModifier;
                                break;
                        case 'M':
                                mods |= Qt::MetaModifier;
                                break;
                        case 'A':
                                mods |= Qt::AltModifier;
                                break;
                        case '-':
                                dashfound = false;
                                break;
                        default:
                                return -1;
                        }
                } else {
                        if (*c == '\\') {
                                switch (*(c+1)) {
                                case '\\':
                                        key = '\\';
                                        break;
                                case 't':
                                        key = '\t';
                                        break;
                                case 's':
                                        key = ' ';
                                        break;
                                default:
                                        return -1;
                                }
                        } else {
                                key = *c;
                        }
                }
        }
        if (!dashfound || !key || !mods)
                return -1;
        else
                return (mods | key);
}
