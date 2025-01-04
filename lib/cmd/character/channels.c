inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("channels");
    set_help_text("The channels command is used to view which communication channels you have access to and their blocked status.");
    set_help_similar(({ "last", "reply", "tell", }));
}

void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    mapping *body = ({ });
    string *list = ({ });

    if (input && tc->query_immortal()) {
        target = determine_immortal_target(tc, input);
    }

    foreach (string channel in sort_array(D_CHANNEL->query_channels(), 1)) {
        list += ({ sprintf("%-15s %s", channel, target->query_channel_blocked(channel) ? "blocked" : "") });
    }
    body += ({ ([
        "header": ({ "Channels" }),
        "columns": 4,
        "items": list,
    ]) });

    list = ({ });
    foreach (string channel in sort_array(D_CHANNEL->query_system_channels(), 1)) {
        list += ({ sprintf("%-15s %s", channel, target->query_channel_blocked(channel) ? "blocked" : "") });
    }
    body += ({ ([
        "header": ({ "System Channels" }),
        "columns": 4,
        "items": list,
    ]) });

    border(([
        "title": "Channels",
        "subtitle": target->query_cap_name(),
        "body": body,
    ]));
}