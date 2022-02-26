void command (string input) {

    write(format_header_bar("WHO", mud_name()) + "\n\n");

    foreach (object user in users()) {
        if (user->query_character()) {
            string characterName = user->query_character()->query_name();
            if (this_character()->query_immortal()) {
                characterName += " (" + user->query_account()->query_name() + ")";
            }
            if (this_user() != user) {
                characterName += " " + time_from_seconds(query_idle(user));
            }
            write("    "+pad_right(characterName, 50) + capitalize(user->query_character()->query_species()+"") + "\n");
        } else {
            write("    ("+user->query_account()->query_name()+" - "+identify(user)+"\n");
        }
    }

    write("\n" + format_footer_bar() + "\n");

}