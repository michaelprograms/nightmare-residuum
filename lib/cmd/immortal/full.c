inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("full [character]");
    set_help_text("The full command is used to return a character's vitals and body to full health.");
}

void command (string input, mapping flags) {
    object tc = this_character(), target = tc;

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if (present(input, environment(tc))) target = present(input, environment(tc));
    }

    message("action", "Character returned to full health.", tc);
    if (target != tc) {
        message("action", tc->query_cap_name() + " returns you to full health.", target);
    }

    // heal vitals
    target->set_hp(target->query_max_hp());
    target->set_sp(target->query_max_sp());
    target->set_mp(target->query_max_mp());

    // heal limbs
    foreach (string l in target->query_limbs()) {
        mapping limb = target->query_limb(l);
        if (limb["damage"] > 0) {
            target->handle_limb_heal(l, limb["damage"]);
            message("action", "Your " + l + " is fully healed.", target);
        }
    }

    // restore limbs
    foreach (string l in target->query_severed_limbs()) {
        target->handle_limb_restore(l);
        message("action", "Your " + l + " is replaced.", target);
    }
}