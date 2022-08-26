inherit STD_LIVING;

nosave private string *__AbilityList;
nosave private int __AbilityChance;

int is_npc () { return 1; }

void create () {
    ::create();
    if (!clonep()) return;
    set_heart_beat(1);
}

void set_level (int l) {
    ::set_level(l);

    set_stat("strength", l * 4);
    set_stat("perception", l * 5);
    set_stat("endurance", l * 5);
    set_stat("charisma", l * 3);
    set_stat("intelligence", l * 4);
    set_stat("agility", l * 5);
    set_stat("luck", to_int(l * 2.5) + secure_random(l + 1));

    set_skill("blade attack", l * 3);
    set_skill("blade defense", l * 2);
    set_skill("blunt attack", l * 3);
    set_skill("blunt defense", l * 2);
    set_skill("knife attack", l * 3);
    set_skill("knife defense", l * 2);
    set_skill("melee attack", l * 3);
    set_skill("melee defense", l * 2);
    set_skill("psionic attack", l * 3);
    set_skill("psionic defense", l * 2);
    set_skill("projectile attack", l * 3);
    set_skill("projectile defense", l * 2);
    set_skill("ranged attack", l * 3);
    set_skill("ranged defense", l * 2);

    update_vitals(1);
}

string *query_ability_list () {
    return __AbilityList;
}
void set_ability_list (string *abilities) {
    __AbilityList = abilities;
}
int query_ability_chance () {
    return __AbilityChance;
}
void set_ability_chance (int chance) {
    __AbilityChance = chance;
}

void heart_beat () {
    living::heart_beat();

    if (time() % 2 || !(query_target_hostile())) return;
    if (this_object()->query_disable()) return;

    if (this_object()->query_ability_chance() > 1+random(100) && sizeof(query_ability_list())) {
        string ability = element_of(query_ability_list());
        this_object()->do_command(ability);
    }
}