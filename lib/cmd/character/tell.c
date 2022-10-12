void command (string input, mapping flags) {
    object tc = this_character(), target;
    string name, msg, myMsg, yourMsg;

    input = strip_colour(input);
    sscanf(input, "%s %s", name, msg);
    if (!sizeof(name) || !sizeof(msg) || !(target = find_character(name))) {
        message("action", "Syntax: <tell [character] [message]>\n", tc);
        return;
    }

    myMsg = "You tell " + target->query_cap_name() + ": " + msg;
    yourMsg = tc->query_cap_name() + " tells you: " + msg;

    message("tell", myMsg + "\n", tc);
    message("tell", yourMsg + "\n", target);

    tc->add_tell_history(myMsg);
    target->add_tell_history(yourMsg);
    target->set_tell_reply(tc->query_name());
}