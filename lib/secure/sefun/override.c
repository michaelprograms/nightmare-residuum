object this_character ();

nomask varargs int input_to () {
    error("efun::input_to disabled");
}

nomask object this_player (int flag) {
    error("efun::this_player disabled");
}

object *users () {
    return filter(efun::users(), (: $1->query_account() && $1->query_account()->query_name() :));
}

nomask void write (string msg) {
    if (this_character() && this_character()->query_key_name() != "test") {
        this_character()->receive_message("system", msg);
    } else if (this_user()) {
        this_user()->receive_message("system", msg);
    } else {
        if (msg[<1] == '\n') msg = msg[0..<2];
        debug_message(msg);
    }
}

varargs void message (mixed type, string message, mixed target, mixed exclude) {
    debug_message(sprintf("message %O %O %O %O", type, message, target, exclude));
    if (!sizeof(message) || (!objectp(target) && !arrayp(target))) {
        return;
    }
    efun::message(type, message, target, exclude);
}

// say

// shout

// potential override:
// userp
// if (ob->query_user()) return ::userp(ob->query_user())