void command (string input) {
    object tc = this_character();

    write(format_header_bar("SCORE") + "\n\n");

    write("Name: " + tc->query_name() + "\n");
    write("Account: " + tc->query_owner() + "\n");
    write("Species: " + tc->query_species() + "\n");
    write("Level: " + tc->query_level() + "\n");
    write("Created: " + ctime(tc->query_created()) + "\n");
    write("Last Action: " + time_ago(tc->query_last_action()) + "\n");

    write("\nVictories: " + tc->query_victory() + "\n");
    write("Defeats: " + tc->query_defeat() + "\n");

    write("Experience: " + tc->query_experience() + "\n");
    write("Total Experience: " + tc->query_total_experience() + "\n");

    write("\n" + format_footer_bar() + "\n");
}