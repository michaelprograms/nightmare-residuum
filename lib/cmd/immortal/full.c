void command (string input, mapping flags) {
    object tc = this_character(), target = tc;

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if (present(input, environment(tc))) target = present(input, environment(tc));
    }

    target->set_hp(target->query_max_hp());
    target->set_sp(target->query_max_sp());
    target->set_mp(target->query_max_mp());
    message("action", "Vitals returned to full.\n", tc);
    if (target != tc) {
        message("action", tc->query_cap_name() + " returns your vitals to full.\n", target);
    }
}