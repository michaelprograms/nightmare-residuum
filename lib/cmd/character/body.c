void command (string input) {
    object tc = this_character(), target = tc;

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if(present(input, environment(tc))) target = present(input, environment(tc));
    }

    write(format_header_bar("BODY", (target != tc ? target->query_name() : 0)) + "\n\n");

    foreach (string limb in target->query_limbs()) {
        write(sprintf("  %-15s", limb) + "\n");
    }

    write("\n" + format_footer_bar() + "\n");
}