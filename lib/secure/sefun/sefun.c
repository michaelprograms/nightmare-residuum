#include "sefun.h"

inherit "/secure/sefun/access.c";
inherit "/secure/sefun/array.c";
inherit "/secure/sefun/border.c";
inherit "/secure/sefun/color.c";
inherit "/secure/sefun/combat.c";
inherit "/secure/sefun/format.c";
inherit "/secure/sefun/grammar.c";
inherit "/secure/sefun/override.c";
inherit "/secure/sefun/path.c";
inherit "/secure/sefun/string.c";
inherit "/secure/sefun/time.c";
inherit "/secure/sefun/user.c";

string driver_version () {
    mixed *lt = localtime(to_int(pcre_extract(__VERSION__, "^fluffos ([0-9]+)\\-")[0]));
    return sprintf("%d-%2'0'd-%2'0'd", lt[5], lt[4]+1, lt[3]);
}

int driver_port () {
    return __PORT__;
}

string mudlib_version () {
    return "NR v0.3";
}

string mud_name () {
    string name = "Nightmare Residuum";
#ifdef MUDNAME
    name = MUD_NAME;
#endif
    return name;
}