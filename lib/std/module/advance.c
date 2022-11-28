#define MAX_LEVEL 10

void create () {
    parse_init();
    parse_add_rule("advance", "");
}

mixed can_advance () {
    return environment(this_character()) == this_object();
}
void do_advance () {
    object po = previous_object();
    int level, curExp, levelCost;

    level = po->query_level();
    curExp = po->query_experience();
    levelCost = D_EXPERIENCE->query_level_cost(level);

    if (level >= MAX_LEVEL) {
        message("action", "You can not advance beyond your current level.", po);
        return;
    }
    if (curExp < levelCost) {
        message("action", "You do not have enough experience to advance to level " + (level + 1) + ".", po);
        return;
    }

    po->spend_experience(levelCost);
    po->set_level(level + 1);
    message("action", "You advance to level " + po->query_level() + ".", po);
    message("action", po->query_cap_name() + " advances in level.", environment(po), po);
}