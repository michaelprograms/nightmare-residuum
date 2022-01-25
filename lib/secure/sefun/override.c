#include "sefun.h";

nomask varargs int input_to () {
    error("efun::input_to disabled");
}

nomask object this_player (int flag) {
    error("efun::this_player disabled");
}

object *users () {
    return filter_array(efun::users(), (: $1->query_account() && $1->query_account()->query_name() :));
}

nomask void write (string msg) {
    if (this_character()) {
        this_character()->receive_message("wrap", msg);
    } else if (this_user()) {
        this_user()->receive_message("system", msg);
        // tell(this_user(), msg);
    } else {
        if (msg[<1] == '\n') msg = msg[0..<2];
        debug_message(msg); // "]"
    }
}

// say

// shout

// potential override:
// userp
// if (ob->query_user()) return ::userp(ob->query_user())