string *__ChannelsBlocked = ({ });

private void initialize_channels () {
    if (!arrayp(__ChannelsBlocked)) {
        __ChannelsBlocked = ({ });
    }
}

string *query_channels_available () {
    initialize_channels();
    return D_CHANNEL->query_channels() - __ChannelsBlocked;
}
string *query_channels_blocked () {
    initialize_channels();
    return __ChannelsBlocked;
}
int query_channel_blocked (string channel) {
    initialize_channels();
    return member_array(channel, __ChannelsBlocked) > -1;
}
int toggle_channel_blocked (string channel) {
    initialize_channels();
    if (member_array(channel, D_CHANNEL->query_channels() + D_CHANNEL->query_system_channels()) == -1) {
        return 0;
    } else if (member_array(channel, __ChannelsBlocked) > -1) {
        __ChannelsBlocked -= ({ channel });
        return 0;
    } else {
        __ChannelsBlocked += ({ channel });
        return 1;
    }
}