void command (string input, mapping flags) {
    object tc = this_character(), target;
    string name, msg;

    input = strip_colour(input);
    if (sscanf(input, "%s %s", name, msg) != 2 ||
      (name == "" || !name) ||
      (!(target = find_character(name)) || tc == target) ||
      (msg == "" || !msg)) {
        message("action", "Syntax: <tell [character] [message]>\n", tc);
        return;
    }

    message("tell", "You tell " + target->query_cap_name() + ": " + msg + "\n", tc);
    message("tell", tc->query_cap_name() + " tells you: " + msg + "\n", target);
}