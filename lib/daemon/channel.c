private string *__Channels;
private string *__SystemChannels;
private mapping __History = ([ ]);

/* ----- helpers ----- */

string *query_channels () {
    return __Channels;
}
string *query_system_channels () {
    return __SystemChannels;
}
int query_valid_channel (string channel) {
    if (channel[<1] == ':') {
        channel = channel[0..<2];
    } else if (channel[<5..] == "emote") {
        channel = channel[0..<6];
    }
    return member_array(channel, __Channels) > -1 || member_array(channel, __SystemChannels) > -1;
}

string *query_history (string channel) {
    return __History[channel];
}
void add_history (string channel, string msg) {
    if (!arrayp(__History[channel])) {
        __History[channel] = ({ });
    }
    __History[channel] += ({ msg });
    if (sizeof(__History[channel]) > 20) {
        __History[channel] = __History[channel][<20..];
    }
}

private string format_channel_name (string channel) {
    int flag = member_array(channel, query_channels()) > -1;
    return (flag ? "[[" : "((") + channel + (flag ? "]]" : "))");
}

object *query_listeners (string channel) {
    return filter(characters(), (: !$1->query_channel_blocked($(channel)) :));
}

private void handle_send (string name, string channel, string msg, int emote, int ipc) {
    string type = "channel", m;

    if (channel == "error") {
        type = "channel error";
    }
    if (emote) {
        m = format_channel_name(channel) + " " + (name ? name + " " : "") + msg;
    } else {
        m = (name ? name + " " : "") + format_channel_name(channel) + " " + msg;
    }

    message(type, m, this_object()->query_listeners(channel));
    add_history(channel, m);
    // if (channel != "error" && !ipc) {
    //     D_IPC->send("CHAT:" + channel + ":" + (name ? name + ":" : "") + msg);
    // }
}

/* ----- send to channel ----- */

void send_ipc (string channel, string name, string msg) {
    if (!channel || !name || !msg) {
        return;
    }

    handle_send(name, channel, msg, 0, 1);
}

void send (string channel, object source, string msg) {
    int emote;

    if (!channel) {
        return;
    }
    if (member_array(channel, __Channels + __SystemChannels) == -1) {
        return;
    }

    if (channel[<1] == ':') {
        channel = channel[0..<2];
        emote = 1;
    } else if (channel[<5..] == "emote") {
        channel = channel[0..<6];
        emote = 1;
    }
    if (msg && msg[0] == ':') {
        msg = msg[1..];
        emote = 1;
    }

    if (!msg) {
        source->toggle_channel_blocked(channel);
        if (source->query_channel_blocked(channel)) {
            message("channel",  "Channel " + format_channel_name(channel) + " is now blocked.", source);
        } else {
            message("channel",  "Channel " + format_channel_name(channel) + " is no longer blocked.", source);
        }
        return;
    } else if (msg && member_array(channel, __SystemChannels) > -1) {
        message("channel",  "Channel " + format_channel_name(channel) + " is read only.", source);
        return;
    }
    if (source->query_channel_blocked(channel)) {
        source->toggle_channel_blocked(channel);
        message("channel",  "Channel " + format_channel_name(channel) + " is no longer blocked.", source);
    }
    handle_send(source->query_cap_name(), channel, msg, emote, 0);
}

void send_system (string channel, string msg) {
    if (member_array(channel, __SystemChannels) == -1) {
        return;
    }
    handle_send(0, channel, msg, 0, 0);
}

/* ----- applies ----- */

void create () {
    if (!__Channels) {
        __Channels = ({ "chat", "newbie", });
    }
    if (!__SystemChannels) {
        __SystemChannels = ({ "connection", "error", });
    }
}