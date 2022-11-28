private string *__Channels;
private string *__SystemChannels;

string *query_channels () {
    return __Channels;
}
string *query_system_channels () {
    return __SystemChannels;
}

int query_valid_channel (string channel) {
    return member_array(channel, __Channels) > -1 || member_array(channel, __SystemChannels) > -1;
}

void create () {
    if (!__Channels) {
        __Channels = ({ "chat", "newbie", });
    }
    if (!__SystemChannels) {
        __SystemChannels = ({ "connection" });
    }
}

private string format_channel_name (string channel) {
    return (member_array(channel, query_channels()) > -1 ? "[[" : "((") + channel + (member_array(channel, query_channels()) > -1 ? "]]" : "))" );
}

private void handle_send (string name, string channel, string msg) {
    string *listeners = filter_array(characters(), (: !$1->query_channel_blocked($(channel)) :));
    message("channel", (name ? name + " " : "") + format_channel_name(channel) + " " + msg  + "\n", listeners);
}

void send (string channel, string msg) {
    if (!channel || member_array(channel, __Channels + __SystemChannels) == -1) return;

    if (!msg) {
        this_character()->toggle_channel_blocked(channel);
        if (this_character()->query_channel_blocked(channel)) {
            message("channel",  "Channel " + format_channel_name(channel) + " is now blocked.\n", this_character());
        } else {
            message("channel",  "Channel " + format_channel_name(channel) + " is no longer blocked.\n", this_character());
        }
        return;
    } else if (msg && member_array(channel, __SystemChannels) > -1) {
        message("channel",  "Channel " + format_channel_name(channel) + " is read only.\n", this_character());
        return;
    }
    if (this_character()->query_channel_blocked(channel)) {
        this_character()->toggle_channel_blocked(channel);
        message("channel",  "Channel " + format_channel_name(channel) + " is no longer blocked.\n", this_character());
    }
    handle_send(this_character()->query_cap_name(), channel, msg);
}

void send_system (string channel, string msg) {
    if (member_array(channel, __SystemChannels) == -1) return;
    handle_send(0, channel, msg);
}