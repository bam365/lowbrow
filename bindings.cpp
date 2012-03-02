#include "bindings.h"
#include "lbmain.h"


Bindings::Bindings()
{
}


void Bindings::add(char*, Command*)
{
}


Command* Bindings::find(char*)
{
        return NULL;
}


Command* Bindings::findmod(short c, short mods)
{
        return NULL;
}


int Bindings::modkey(char* str)
{
        short mods, key;
        int i, l;

        l = strlen(str);
        if (l < 4 || !(str[0] == '<' && str[l - 1] == '>'))
                return -1;
        for (i = 1; i < l - 1; i++) {
                switch (str[i]) {
