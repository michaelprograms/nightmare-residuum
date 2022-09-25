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
    int n = l + 1;

    set_stat("strength", n * 4);
    set_stat("perception", n * 5);
    set_stat("endurance", n * 5);
    set_stat("charisma", n * 3);
    set_stat("intelligence", n * 4);
    set_stat("agility", n * 5);
    set_stat("luck", to_int(n * 2.5) + random(n + 1));

    set_skill("blade attack", n * 4);
    set_skill("blade defense", n * 3);
    set_skill("blunt attack", n * 4);
    set_skill("blunt defense", n * 3);
    set_skill("knife attack", n * 4);
    set_skill("knife defense", n * 3);
    set_skill("melee attack", n * 4);
    set_skill("melee defense", n * 3);
    set_skill("psionic attack", n * 4);
    set_skill("psionic defense", n * 3);
    set_skill("projectile attack", n * 4);
    set_skill("projectile defense", n * 3);
    set_skill("ranged attack", n * 4);
    set_skill("ranged defense", n * 3);

    ::set_level(l);
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

void handle_ability_attack () {
    if (this_object()->query_ability_chance() > 1+random(100) && sizeof(query_ability_list())) {
        string ability = element_of(query_ability_list());
        this_object()->do_command(ability);
    }
}

nosave private mapping __Inventory = ([ ]);
void set_inventory (mapping inventory) {
    if (!mapp(inventory)) error("Bad argument 1 to npc->set_inventory");

    __Inventory = inventory;
}
mapping query_inventory () {
    return __Inventory;
}

void handle_received (object env) {
    ::handle_received(env);

    if (sizeof(__Inventory)) {
        foreach (string item,string command in __Inventory) {
            new(item)->handle_move(this_object());
            do_command(command);
        }
        __Inventory = ([ ]);
    }
}