inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("last [tell|say|(channel)]");
    set_help_text("The last command is used to view the history of your character's tells, says, or a channel your character has access to.");
    set_help_similar(({ "channels", "reply", "tell", }));
}

void command (string input, mapping flags) {
    object tc = this_character();
    string *msgs;
    string *body = ({ }), subtitle;

    if (input == "tells" || input == "tell") {
        if (!sizeof(msgs = tc->query_tell_history())) {
            body += ({ "No one has told you anything." });
        } else {
            body += map(msgs, (: format_message_color("tell", $1) :));
        }
        subtitle = sizeof(msgs) + " tell" + (sizeof(msgs) != 1 ? "s" : "");
    } else if (input == "says" || input == "say") {
        if (!sizeof(msgs = tc->query_say_history())) {
            body += ({ "No one has said anything to you." });
        } else {
            body += map(msgs, (: format_message_color("say", $1) :));
        }
        subtitle = sizeof(msgs) + " say" + (sizeof(msgs) != 1 ? "s" : "");
    } else if (D_CHANNEL->query_valid_channel(input)) {
        string type = (input == "error" ? "channel error" : "channel");

        if (!sizeof(msgs = D_CHANNEL->query_history(input))) {
            body += ({ "No history for " + input + " channel." });
        } else {
            body += map(msgs, (: format_message_color($(type), $1) :));
        }
        subtitle = sizeof(msgs) + " message" + (sizeof(msgs) != 1 ? "s" : "");
    } else {
        message("action", "Syntax: " + query_syntax(), tc);
        return;
    }

    border(([
        "title": "LAST",
        "subtitle": subtitle,
        "body": ([
            "items": body,
            "align": "left",
            "columns": 1,
        ]),
    ]));
}