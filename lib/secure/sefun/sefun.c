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
inherit "/secure/sefun/roll.c";
inherit "/secure/sefun/string.c";
inherit "/secure/sefun/time.c";
inherit "/secure/sefun/user.c";

string driver_version () {
    return explode(__VERSION__, "-")[0];
}

int driver_port () {
    return __PORT__;
}

string mudlib_version () {
    return "NR v0.3";
}

string mud_name () {
    string name = "Nightmare Residuum";
#ifdef MUD_NAME
    name = MUD_NAME;
#endif
    return name;
}

string call_trace () {
    int i;
    string *programs = call_stack(0);
    object *objects = call_stack(1);
    string *functions = call_stack(2);
    string *origins = call_stack(3);
    string *lines = map(call_stack(4), function (string line) {
        int num;
        sscanf(line, "%*s:%d", num);
        return sprintf("%d", num);
    });
    string res = "";

    i = sizeof(programs);
    while(i --) {
        res += sprintf("[%O] %s:%s::%s() (%s)\n", objects[i], programs[i], lines[i], functions[i], origins[i]);
    }

    return res;
}