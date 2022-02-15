private string *__Channels;
private string *__SystemChannels;

string *query_channels () {
    return __Channels;
}
string *query_system_channels () {
    return __SystemChannels;
}

int query_valid_channel (string channel) {
    return member_array(channel, __Channels) > -1 && member_array(channel, this_character()->query_channels()) > -1;
}

void create () {
    if (!__Channels) {
        __Channels = ({ "chat" });
    }
    if (!__SystemChannels) {
        __SystemChannels = ({ "connection" });
    }
}

private void handle_send (string name, string channel, string msg) {
    string info = (name ? name + " [" : "(") + channel + (name ? name "]" : ")");
    message("channel", info + " " + msg  + "\n", users());
}

void send (string channel, string msg) {
    string name;

    if (!channel || member_array(channel, __Channels) == -1) return;

    name = this_character()->query_name();
    handle_send(name, channel, msg);
}

void send_system (string channel, string msg) {
    if (member_array(channel, __SystemChannels) == -1) return;
    handle_send(0, channel, msg);
}