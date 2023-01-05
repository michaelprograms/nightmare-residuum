#define STATS ({"strength","perception","endurance","charisma","intelligence","agility","luck"})

inherit STD_COMMAND;

void create () {
    set_syntax("improve [stat]");
    set_help_text("The improve command is used to spend experience points to permanently increase one of your character's stats.");
}

void command (string input, mapping flags) {
    object tc = this_character();
    string stat;
    int cost;

    if (!input) {
        message("action", "Syntax: " + format_syntax("train [stat]"), tc);
        return;
    }
    stat = format_stat_verbose(input);
    if (!stat || member_array(stat, STATS) == -1) {
        message("action", "Syntax: " + format_syntax("train [stat]"), tc);
        return;
    }

    cost = D_EXPERIENCE->query_stat_cost(stat, tc->query_stat_base(stat), tc->query_class(), tc->query_species());
    if (tc->query_experience() < cost) {
        message("action", "You are not yet ready to improve your " + stat + ".", tc);
        return;
    }

    if (tc->query_stat_base(stat) >= D_CLASS->query_max_stat(tc->query_class(), stat, tc->query_level())) {
        message("action", "Your " + stat + " is already at the maximum.", tc);
        return;
    }

    tc->add_experience(-cost);
    tc->set_stat(stat, tc->query_stat_base(stat) + 1);
    message("action", "You train your " + stat + ", improving it to " + tc->query_stat_base(stat) + ".", tc);
}