#ifndef BINDINGS_H
#define BINDINGS_H

#include <map>
#include <string>

//Forward decl
class Command;


class Bindings
{
public:
        Bindings();

        bool add(char*, Command*);
        Command* find(const char*);
        Command* findmod(int mods, int k);
        bool find_startswith(const char*);

        static int modkey(char*);

protected:
        bool is_mod_binding(char*);

private:

        std::map<int, Command*> m_modcmds;
        std::map<std::string, Command*> m_cmds;
};

#endif // BINDINGS_H
