/**
 * Disabled: Causes the next line of input to be sent to the specified function.
 *
 * @param args
 */
nomask varargs int input_to (mixed args...) {
    error("efun::input_to disabled");
}

/**
 * Disabled: The current player object.
 * Use this_user() instead.
 *
 * @param flag
 */
nomask varargs object this_player (int flag) {
    error("efun::this_player disabled");
}

/**
 * Returns a list of interactive users that have an account set.
 * This excludes the login prompt before account name input.
 *
 * @returns {STD_USER} an array of interactive user accounts
 */
object *users () {
    return filter(efun::users(), (: $1 && $1->query_name() :));
}

/**
 * Write text to the current user or to the debug log.
 *
 * @param msg the text to receive or display
 */
nomask void write (string msg) {
    if (SEFUN->this_character() && SEFUN->this_character()->query_key_name() != "test") {
        SEFUN->this_character()->receive_message("system", msg);
    } else if (efun::this_user()) {
        efun::this_user()->receive_message("system", msg);
    } else {
        if (msg[<1] == '\n') {
            msg = msg[0..<2];
        }
        debug_message(msg);
    }
}

/**
 * Deliver typed messages to the targets.
 *
 * @param type the type of message
 * @param message text to display to targets
 * @param target the recipient(s) of message
 * @param exclude an exclusion list of recipients
 */
varargs void message (mixed type, string message, mixed target, mixed exclude) {
    if (!sizeof(message) || (!objectp(target) && !arrayp(target))) {
        return;
    }
    if (!arrayp(exclude) && !objectp(exclude)) {
        exclude = ({ });
    }
    efun::message(type, message, target, exclude);
}