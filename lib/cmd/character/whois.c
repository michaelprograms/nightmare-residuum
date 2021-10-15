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
    // write("Account: " + char->query_account() + "\n");
    write("Type: " + char->query_type() + "\n");
    write("Created: " + time_ago(char->query_created(), 4) + "\n");
    write("Last Action: " + time_ago(char->query_last_action()) + "\n");

    write("\n" + format_footer_bar() + "\n");
    destruct(char);
}