inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("reply");
    set_help_text("The reply command is used to reply to a tell that has been sent to your character.");
    set_help_similar(({ "channels", "last", "tell", }));
}

void command (string input, mapping flags) {
    object tc = this_character(), target;
    string name, msg, myMsg, yourMsg;

    msg = strip_colour(input);
    if (!sizeof(msg)) {
        message("action", "Syntax: <reply [message]>", tc);
        return;
    }
    name = tc->query_tell_reply();
    if (!sizeof(name) || !(target = find_character(name)) || tc == target) {
        message("action", "You have no one to reply to.", tc);
        return;
    }

    myMsg = "You reply to " + target->query_cap_name() + ": " + msg;
    yourMsg = tc->query_cap_name() + " replies: " + msg;

    message("tell", myMsg, tc);
    message("tell", yourMsg, target);

    tc->add_tell_history(myMsg);
    target->add_tell_history(yourMsg);
    target->set_tell_reply(tc->query_name());
}