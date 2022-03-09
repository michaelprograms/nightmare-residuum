void command (string input) {

    write(format_header_bar("WHO", mud_name()) + "\n\n");

    foreach (object user in users()) {
        object char;
        if (char = user->query_character()) {
            string characterName = char->query_cap_name();
            if (this_character()->query_immortal()) {
                characterName += " (" + user->query_account()->query_name() + ")";
            }
            if (this_user() != user) {
                characterName += " " + time_from_seconds(query_idle(user));
            }
            write("    "+sprintf("%-50s", characterName) + sprintf("%-16s", capitalize(char->query_species())) + " " + sprintf("%3d", char->query_level()) + "\n");
        } else {
            write("    ("+user->query_account()->query_name()+" - "+identify(user)+"\n");
        }
    }

    write("\n" + format_footer_bar() + "\n");

}