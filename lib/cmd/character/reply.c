void command (string input, mapping flags) {
    object tc = this_character(), target;
    string name, msg, myMsg, yourMsg;

    msg = strip_colour(input);
    if (!sizeof(msg)) {
        message("action", "Syntax: <reply [message]>\n", tc);
        return;
    }
    name = tc->query_tell_reply();
    if (!sizeof(name) || !(target = find_character(name)) || tc == target) {
        message("action", "You have no one to reply to.\n", tc);
        return;
    }

    myMsg = "You reply to " + target->query_cap_name() + ": " + msg;
    yourMsg = tc->query_cap_name() + " replies: " + msg;

    message("tell", myMsg + "\n", tc);
    message("tell", yourMsg + "\n", target);

    tc->add_tell_history(myMsg);
    target->add_tell_history(yourMsg);
    target->set_tell_reply(tc->query_name());
}