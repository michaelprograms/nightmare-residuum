void command (string input) {
    object tc = this_character(), target = tc;
    string *limbs, *sorted;

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if(present(input, environment(tc))) target = present(input, environment(tc));
    }

    write(format_header_bar("BODY", (target != tc ? target->query_cap_name() : 0)) + "\n\n");

    limbs = target->query_limbs();
    sorted = ({ });
    foreach (string type in ({"head","torso","arm","hand","leg","tail","foot","paw",})) {
        foreach (string l in sort_array(filter_array(limbs, (: regexp($1, $(type)) :)), 1)) {
            mapping limb = target->query_limb(l);
            write(sprintf("  %-16s %3s", l, (limb["damage"]*100/limb["maxdamage"])+"%") + "\n");
        }
    }

    write("\n" + format_footer_bar() + "\n");
}