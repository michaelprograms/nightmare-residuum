#include "sefun.h"

inherit "/secure/sefun/array.c";
inherit "/secure/sefun/banner.c";
inherit "/secure/sefun/border.c";
inherit "/secure/sefun/color.c";
inherit "/secure/sefun/convert.c";
inherit "/secure/sefun/combat.c";
inherit "/secure/sefun/format.c";
inherit "/secure/sefun/grammar.c";
inherit "/secure/sefun/json.c";
inherit "/secure/sefun/noise.c";
inherit "/secure/sefun/override.c";
inherit "/secure/sefun/path.c";
inherit "/secure/sefun/roll.c";
inherit "/secure/sefun/string.c";
inherit "/secure/sefun/time.c";
inherit "/secure/sefun/user.c";

string version () {
    string v = __VERSION__;
    v = explode(__VERSION__, "-")[0];
    v = replace_string(v, " ", " v");
    v = v[0..<5] + "." + v[<4..<3] + "." + v[<2..];
    return v;
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
    string *programs = map(call_stack(4), (: replace_string($1, ":", "::") :));
    object *objects = call_stack(1);
    string *functions = call_stack(2);
    string *origins = call_stack(3);
    string *res = ({ });
    int i = sizeof(programs);
    while (i --) {
        res += ({
            sprintf(
                "[%O] %s:%s() (%s)",
                objects[i],
                programs[i],
                functions[i],
                origins[i]
            )
        });
    }
    return implode(res, "\n");
}