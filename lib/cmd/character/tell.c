inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("tell");
    set_help_text("The tell command is used to send a message from your character to another connected character.");
    set_help_similar(({ "channels", "last", "reply", }));
}

void command (string input, mapping flags) {
    object tc = this_character(), target;
    string name, msg, myMsg, yourMsg;

    input = strip_colour(input);
    sscanf(input, "%s %s", name, msg);
    if (!sizeof(name) || !sizeof(msg) || !(target = find_character(name))) {
        message("action", "Syntax: <tell [character] [message]>", tc);
        return;
    }

    myMsg = "You tell " + target->query_cap_name() + ": " + msg;
    yourMsg = tc->query_cap_name() + " tells you: " + msg;

    message("tell", myMsg, tc);
    message("tell", yourMsg, target);

    tc->add_tell_history(myMsg);
    target->add_tell_history(yourMsg);
    target->set_tell_reply(tc->query_name());
}