void command (string input) {
    object tc = this_character();
    string *list = ({});
    int width, columns;

    width = this_user()->query_account()->query_setting("width") || 80;
    columns = width / 40;

    write(format_header_bar("SCORE") + "\n\n");

    list += ({ sprintf("  %-18s %s", "Name:", tc->query_name()) });
    list += ({ sprintf("  %-18s %s", "Account:", this_user()->query_account()->query_name()) });

    list += ({ sprintf("  %-18s %s", "Species:", capitalize(tc->query_species())) });
    list += ({ sprintf("  %-18s %d", "Level:", tc->query_level()) });

    list += ({ sprintf("  %-18s %s", "Created:", strftime("%Y/%m/%u", tc->query_created())) });
    // list += ({ sprintf("  %-18s %s", "Last Action:", time_ago(tc->query_last_action())) });
    list += ({ "" });

    list += ({ "" });
    list += ({ "" });

    list += ({ sprintf("  %-18s %d", "Victories:", tc->query_victory()) });
    list += ({ sprintf("  %-18s %d", "Defeats:", tc->query_defeat()) });

    list += ({ sprintf("  %-18s %d", "Experience:", tc->query_experience()) });
    list += ({ sprintf("  %-18s %d", "Total Experience:", tc->query_total_experience()) });

    // write("  Name: " + tc->query_name() + "\n");
    // write("  Account: " +  + "\n");
    // write("  Species: " + tc->query_species() + "\n");
    // write("Level: " + tc->query_level() + "\n");
    // write("Created: " +  + "\n");
    // write("Last Action: " +  + "\n");

    // write("\nVictories: " + tc->query_victory() + "\n");
    // write("Defeats: " + tc->query_defeat() + "\n");

    // write("Experience: " + tc->query_experience() + "\n");
    // write("Total Experience: " + tc->query_total_experience() + "\n");

    write(format_page(list, columns));

    write("\n" + format_footer_bar() + "\n");
}