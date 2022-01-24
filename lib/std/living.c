#include "living/living.h"

inherit STD_CONTAINER;

inherit "/std/living/body.c";
inherit "/std/living/stats.c";
inherit "/std/living/vitals.c";

private string __Type;
private string __LastEnvironment;

void create () {
    container::create();
}

string query_type () {
    return __Type;
}
void set_type (string type) {
    // @TODO security
    __Type = type;
}
string query_last_environment () {
    return __LastEnvironment;
}
string query_last_location () {
    if (!__LastEnvironment) return 0;
    return __LastEnvironment->query_short();
}
void set_last_location (string location) {
    if (location) {
        __LastEnvironment = location;
    }
}

int handle_move (mixed dest) {
    int move = ::handle_move(dest);
    if (stringp(dest) && dest != "/domain/Nowhere/freezer.c") {
        __LastEnvironment = dest;
    } else if (objectp(dest) && file_name(dest) != "/domain/Nowhere/freezer.c") {
        __LastEnvironment = file_name(dest);
    }
    return move;
}

int handle_go (mixed dest, string method, string dir) {
    int move;
    message("go", "You "+method+" "+dir+".\n", this_object());
    message("go", query_name()+" "+pluralize(method)+" "+dir+".\n", environment()->query_living_contents(), ({ this_object() }));
    move = handle_move(dest);
    message("go", query_name()+" "+pluralize(method)+" in.\n", environment()->query_living_contents(), ({ this_object() }));
    return move;
}

varargs int do_command (string str, int debug) {
    // string action;
    string verb, input;
    mixed result, resultGo;
    sscanf(str, "%s %s", verb, input);

    // action = D_COMMAND->query_command(verb);
    // if(stringp(action)) {
    //     call_other(action, "command", input); // @TODO is this right here?
    // } else
    if (!environment()) {
        return 0;
    }

    result = parse_sentence(str, debug);
    if (stringp(result)) {
        write(result + "\n");
        return 1;
    }

    /*
    ** If the result is 0, the parser didn't know the verb so we keep looking.
    ** If a 1 was returned, then nothing more needs to be done.
    ** If the result is -1 or -2, the parser figured something was wrong.
    **
    */
    switch (result) {
    case 0: // Parser found no verb, fall through
        break;
    case 1: // Parser called verb
        return 1;
    case -1:
        write("Nonsense?!\n");
        return 1;
    case -2:
        write("You aren't able to do that.\n");
        return 1;
    default:
        write("Unexpected result.\n");
        if (undefinedp(result)) debug_message("Result was undefined.");
        else debug_message( "Error was: "+identify(result));
        return 1;
    }
    if (environment() && environment()->query_exit(str)) {
        resultGo = parse_sentence("go "+str);
        if (resultGo == 1) {
            return 1;
        }
        if (!result) {
            result = resultGo;
        }
    }
    if (stringp(result) && (result[0..12] != "You can't go " && result[0..11] != "There is no ")) {
        write(result+"\n");
        return 1;
    }

    return 0;
}

int is_living () {
    return 1;
}