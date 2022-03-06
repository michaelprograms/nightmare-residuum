private string *__ChannelsBlocked = ({ });

string *query_channels_available () {
    if (!arrayp(__ChannelsBlocked)) __ChannelsBlocked = ({ });
    return D_CHANNEL->query_channels() - __ChannelsBlocked;
}
string *query_channels_blocked () {
    if (!arrayp(__ChannelsBlocked)) __ChannelsBlocked = ({ });
    return __ChannelsBlocked;
}
int query_channel_blocked (string channel) {
    if (!arrayp(__ChannelsBlocked)) __ChannelsBlocked = ({ });
    return member_array(channel, __ChannelsBlocked) > -1;
}
int toggle_channel_blocked (string channel) {
    if (member_array(channel, D_CHANNEL->query_channels() + D_CHANNEL->query_system_channels()) == -1) return 0;

    if (member_array(channel, __ChannelsBlocked) > -1) {
        __ChannelsBlocked -= ({ channel });
        return 0;
    } else {
        __ChannelsBlocked += ({ channel });
        return 1;
    }
}