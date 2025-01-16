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
inherit "/secure/sefun/room.c";
inherit "/secure/sefun/string.c";
inherit "/secure/sefun/time.c";
inherit "/secure/sefun/user.c";

/**
 * Sefun to access human readable format of driver version.
 *
 * @returns driver version text in v0.0.0 format
 */
string version () {
    string v = __VERSION__;
    v = explode(__VERSION__, "-")[0];
    v = replace_string(v, " ", " v");
    v = v[0..<5] + "." + v[<4..<3] + "." + v[<2..];
    return v;
}

/**
 * Sefun to access the driver's primary port.
 *
 * @returns port as an integer
 */
int driver_port () {
    return __PORT__;
}

/**
 * Sefun to access mudlib's version.
 *
 * @returns mudlib version text
 */
string mudlib_version () {
    return "NR v0.3";
}

/**
 * Sefun to access the mud name.
 *
 * @returns mudlib name text
 */
string mud_name () {
    string name = "Nightmare Residuum";
#ifdef MUD_NAME
    name = MUD_NAME;
#endif
    return name;
}

/**
 * Sefun to print a detailed call stack trace.
 *
 * @returns multi-line string of the current call stack with some formatting
 */
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