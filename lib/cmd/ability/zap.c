inherit STD_ABILITY;

void command (string input) {
    object tc = this_character(), target = tc->query_target_hostile();
    int damage;

    // @TODO find target from input

    if (!target) {
        message("action", "You have no current hostile target.\n", tc);
        return;
    }

    damage = calculate_damage(tc, target, "psionic");
    message("action", "You zap " + target->query_cap_name() + "!\n", tc);
    message("action", tc->query_cap_name() + " zaps you!\n", target);
    message("action", tc->query_cap_name() + " zaps " + target->query_cap_name() + "!\n", environment(tc), ({ tc, target }));
    target->handle_damage(damage, tc);
    // @TODO train skills
    // @TODO use mp
}