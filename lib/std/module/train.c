#define STATS ({"strength","perception","endurance","charisma","intelligence","agility","luck"})

void create () {
    parse_init();
    parse_add_rule("train", "");
    parse_add_rule("train", "STR");
}

mixed can_train () {
    return environment(this_character()) == this_object();
}
void do_train () {
    message("action", "Syntax: " + format_syntax("train [stat]"), this_character());
}
mixed can_train_str (mixed args...) {
    return environment(this_character()) == this_object();
}
void do_train_str (mixed args...) {
    object tc = this_character();
    string stat;
    int cost;

    if (args && sizeof(args) > 0) {
        stat = format_stat_verbose(args[0]);
    }

    if (!stat || member_array(stat, STATS) == -1) {
        return do_train();
    }

    cost = D_EXPERIENCE->query_stat_cost(stat, tc->query_stat_base(stat), tc->query_class(), tc->query_species());
    if (tc->query_experience() < cost) {
        message("action", "You are not yet ready to train your " + stat + " any further.", tc);
        return;
    }

    tc->add_experience(-cost);
    tc->set_stat(stat, tc->query_stat(stat) + 1);
    message("action", "You train your " + stat + ", improving it to " + tc->query_stat(stat) + "!", tc);
}