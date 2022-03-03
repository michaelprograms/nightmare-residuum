void command (string input) {
    object char;

    if (!input || input == "") {
        write("Whois which character name?\n");
        return;
    }

    char = D_CHARACTER->query_whois_character(input);
    if (!char) {
        write("There is no character with that name on " + mud_name() + "\n");
        return;
    }

    write(format_header_bar("WHOIS") + "\n\n");

    write("Name: " + char->query_name() + "\n");
    if (this_character()->query_immortal()) {
        write("Account: " + char->query_account() + "\n");
        // @TODO ip
    }
    write("Species: " + char->query_species() + "\n");
    write("Level: " + char->query_level() + "\n");
    write("Created: " + ctime(char->query_created()) + "\n");
    write("Last Action: " + time_ago(char->query_last_action()) + "\n");

    write("\n" + format_footer_bar() + "\n");
    destruct(char);
}