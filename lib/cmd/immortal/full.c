inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("full [character]");
    set_help_text("The full command is used to return a character's vitals and body to full health.");
}

void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    string *limbs = ({ });

    if (input && tc->query_immortal()) {
        target = determine_immortal_target(tc, input);
    }

    message("action", "Character returned to full health.", tc);
    if (target != tc) {
        message("action", tc->query_cap_name() + " returns you to full health.", target);
    }

    // heal vitals
    target->set_hp(target->query_max_hp());
    target->set_sp(target->query_max_sp());
    target->set_mp(target->query_max_mp());

    // restore severed limbs
    limbs = sort_array(target->query_severed_limbs(), (: $(target)->query_limb($1)["attach"] == $1 ? -1 : $(target)->query_limb($2)["attach"] == $1 ? 1 : 0 :));
    foreach (string l in limbs) {
        if (target->handle_limb_restore(l) == -1) {
            message("action", "Unable to restore your missing " + l + ".", target);
        } else {
            message("action", "Your missing " + l + " is restored.", target);
        }
    }

    // heal limbs
    foreach (string l in target->query_limbs()) {
        mapping limb = target->query_limb(l);
        if (limb["damage"] > 0) {
            target->handle_limb_heal(l, limb["damage"]);
            message("action", "Your " + l + " is fully healed.", target);
        }
    }
}
