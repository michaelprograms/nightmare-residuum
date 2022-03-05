#include "sefun.h"

inherit "/secure/sefun/access.c";
inherit "/secure/sefun/array.c";
inherit "/secure/sefun/color.c";
inherit "/secure/sefun/combat.c";
inherit "/secure/sefun/format.c";
inherit "/secure/sefun/grammar.c";
inherit "/secure/sefun/override.c";
inherit "/secure/sefun/path.c";
inherit "/secure/sefun/string.c";
inherit "/secure/sefun/tell.c";
inherit "/secure/sefun/time.c";
inherit "/secure/sefun/user.c";

int driver_port () {
    return __PORT__;
}

string driver_version () {
    string v = "0.0.0";
#ifdef __VERSION__
    v = __VERSION__;
#endif
    return explode(v, ".")[0];
}

string mudlib_version () {
    return "NR v0.1";
}

string mud_name () {
    string name = "Nightmare Residuum";
#ifdef MUDNAME
    name = MUD_NAME;
#endif
    return name;
}