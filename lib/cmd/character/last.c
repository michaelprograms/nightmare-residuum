void command (string input, mapping flags) {
    object tc = this_character();
    string *msgs;

    if (input == "tells" || input == "tell") {
        if (!sizeof(msgs = tc->query_tell_history())) {
            message("action", "No one has told you anything.\n", tc);
            return;
        }
        message("action", "Last " + sizeof(msgs) + " tell" + (sizeof(msgs) > 1 ? "s" : "") + ":\n", tc);
        foreach (string msg in msgs) {
            message("tell", msg + "\n", tc);
        }
        return;
    } else if (input == "says" || input == "say") {
        if (!sizeof(msgs = tc->query_say_history())) {
            message("action", "No one has said anything.\n", tc);
            return;
        }
        message("action", "Last " + sizeof(msgs) + " say" + (sizeof(msgs) > 1 ? "s" : "") + ":\n", tc);
        foreach (string msg in msgs) {
            message("say", msg + "\n", tc);
        }
        return;
    } else {
        message("action", "Syntax: <last says|tells>\n", tc);
        return;
    }
}