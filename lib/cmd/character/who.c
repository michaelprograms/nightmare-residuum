void command (string input) {
    write(format_header_bar("WHO", mud_name()) + "\n\n");
    foreach (object user in users()) {
        if (user->query_character()) {
            write("    "+pad_right(user->query_character()->query_name()+" ("+user->query_account()->query_name()+")", 50) + capitalize(user->query_character()->query_type()+"") + "\n");
        } else {
            write("    ("+user->query_account()->query_name()+" - "+identify(user)+"\n");
        }
    }
    write("\n" + format_footer_bar() + "\n");
}