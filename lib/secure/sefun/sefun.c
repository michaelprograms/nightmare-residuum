inherit "/secure/sefun/access.c";
inherit "/secure/sefun/array.c";
inherit "/secure/sefun/color.c";
inherit "/secure/sefun/combat.c";
inherit "/secure/sefun/format.c";
inherit "/secure/sefun/grammar.c";
inherit "/secure/sefun/override.c";
inherit "/secure/sefun/path.c";
inherit "/secure/sefun/string.c";
inherit "/secure/sefun/time.c";
inherit "/secure/sefun/user.c";

#include "sefun.h"

int driver_port () {
    return __PORT__;
}

string driver_version () {
    string v = "0.0.0";
#ifdef __VERSION__
    v = __VERSION__;
#endif
    return explode(v, " ")[0];
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

/* ----- ldmud shims ----- */

varargs void message(string type, string message, mixed target, mixed exclude) {
    object *targets = ({ });

    if (objectp(target)) {
        targets += ({ target });
    } else if (pointerp(target)) {
        foreach (object ob in target) {
            targets += ({ ob });
        }
    }
    if (objectp(exclude)) {
        targets -= ({ exclude });
    } else if (pointerp(exclude)) {
        foreach (object ob in exclude) {
            targets -= ({ ob });
        }
    }

    foreach (object ob in targets) {
        ob->receive_message(type, message);
    }
}
string *keys (mapping m) {
    return m_indices(m);
}

object this_user () {
    return this_player();
}

int member (mixed a, mixed b) { return efun::member(b, a); }

object new (string path) { return clone_object(path); }

int uptime () { return time() - __BOOT_TIME__; }

void debug_message (string str) {
    efun::debug_message(str + "\n");
}