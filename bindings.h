#ifndef BINDINGS_H
#define BINDINGS_H

#include <map>

//Forward decl
class Command;

class Bindings
{
public:
        Bindings();

        void add(char*, Command*);
        Command* find(char*);
        Command* findmod(short c, short mods);

        static int modkey(char*);

private:
        std::map<int, Command*> m_modcmds;
        std::map<char*, Command*> m_cmds;
};

#endif // BINDINGS_H
