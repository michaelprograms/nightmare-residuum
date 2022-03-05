void command (string input) {
    object tc = this_character();

    write(format_header_bar("CHANNELS") + "\n\n");

    write("  %^BOLD%^Channels:%^RESET%^\n");
    foreach (string channel in D_CHANNEL->query_channels()) {
        write(sprintf("    %-15s %s\n", channel, tc->query_channel_blocked(channel) ? "Blocked" : ""));
    }
    write("\n  %^BOLD%^System Channels:%^RESET%^\n");
    foreach (string channel in D_CHANNEL->query_system_channels()) {
        write(sprintf("    %-15s %s\n", channel, tc->query_channel_blocked(channel) ? "Blocked" : ""));
    }

    write("\n" + format_footer_bar() + "\n");
}