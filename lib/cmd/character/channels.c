inherit STD_COMMAND;

void create () {
    set_syntax("channels");
    set_help_text("The channels command is used to view which communication channels you have access to and their blocked status.");
    set_help_similar(({ "last", "reply", "tell", }));
}

void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    string subtitle, *body = ({}), *list;

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if (present(input, environment(tc))) target = present(input, environment(tc));
        subtitle = target->query_cap_name();
    }

    list = ({ });
    foreach (string channel in sort_array(D_CHANNEL->query_channels(), 1)) {
        list += ({ sprintf("%-15s %s", channel, target->query_channel_blocked(channel) ? "blocked" : "") });
    }
    body += ({ ([
        "header": "Channels",
        "columns": 2,
        "items": list,
    ]) });

    list = ({ });
    foreach (string channel in sort_array(D_CHANNEL->query_system_channels(), 1)) {
        list += ({ sprintf("%-15s %s", channel, target->query_channel_blocked(channel) ? "blocked" : "") });
    }
    body += ({ ([
        "header": "System Channels",
        "columns": 2,
        "items": list,
    ]) });

    border(([
        "title": "Channels",
        "subtitle": subtitle,
        "body": body,
    ]));
}