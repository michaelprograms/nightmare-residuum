void command (string input, mapping flags) {
    object tc = this_character(), target;
    string name, msg;

    msg = strip_colour(input);
    name = tc->query_property("reply");
    if ((name == "" || !name) ||
    (!(target = find_character(name)) || tc == target) ||
    (msg == "" || !msg)) {
        message("action", "You do not have someone to reply to.\n", tc);
        return;
    }

    message("tell", "You reply to " + target->query_cap_name() + ": " + msg + "\n", tc);
    message("tell", tc->query_cap_name() + " replies to you: " + msg + "\n", target);
}