private string *__ChannelsBlocked = ({ });

string *query_channels_available () {
    if (!pointerp(__ChannelsBlocked)) __ChannelsBlocked = ({ });
    return D_CHANNEL->query_channels() - __ChannelsBlocked;
}
string *query_channels_blocked () {
    if (!pointerp(__ChannelsBlocked)) __ChannelsBlocked = ({ });
    return __ChannelsBlocked;
}
int query_channel_blocked (string channel) {
    if (!pointerp(__ChannelsBlocked)) __ChannelsBlocked = ({ });
    return member(channel, __ChannelsBlocked) > -1;
}
int toggle_channel_blocked (string channel) {
    if (member(channel, D_CHANNEL->query_channels() + D_CHANNEL->query_system_channels()) == -1) return 0;

    if (member(channel, __ChannelsBlocked) > -1) {
        __ChannelsBlocked -= ({ channel });
        return 0;
    } else {
        __ChannelsBlocked += ({ channel });
        return 1;
    }
}