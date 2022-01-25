void command (string input) {
    write(format_header_bar("TIME", mud_name()) + "\n\n");

    write("  %^BOLD%^" + sprintf("%-18s", "Servertime:") + "%^RESET%^ " + ctime(time()) + "\n");
    write("  %^BOLD%^" + sprintf("%-18s", "Uptime:") + "%^RESET%^ " + time_from_seconds(uptime()) + "\n");

    write("\n" + format_footer_bar() + "\n");
}