inherit STD_LIVING;

nosave private string *__AbilityList;
nosave private int __AbilityChance;
nosave private int __Aggressive;

int is_npc () { return 1; }

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
    set_skill("brawl attack", n * 4);
    set_skill("brawl defense", n * 3);
    set_skill("blunt attack", n * 4);
    set_skill("blunt defense", n * 3);
    set_skill("psionic attack", n * 4);
    set_skill("psionic defense", n * 3);
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
    if (!sizeof(query_ability_list())) {
        return;
    }

    if (query_ability_chance() > 1+random(100)) {
        string ability = element_of(query_ability_list());
        this_object()->do_command(ability);
    }
}

int query_aggressive () {
    return __Aggressive;
}
void set_aggressive (int n) {
    __Aggressive = n;
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

/* ----- applies ----- */

void create () {
    ::create();
    if (!clonep()) return;
    set_heart_beat(1);
}

void handle_receive_living_in_env (object living) {
    if (living->is_character() && __Aggressive > 0 && !query_hostile(living)) {
        if (living->query_stat("charisma") < __Aggressive) {
            message("action", "%^RED%^BOLD%^You attack " + living->query_cap_name() + "!%^RESET%^", this_object());
            message("action", "%^RED%^BOLD%^" + this_object()->query_cap_name() + " attacks you!%^RESET%^", living);
            message("action", "%^RED%^BOLD%^" + this_object()->query_cap_name() + " attacks " + living->query_cap_name() + "!%^RESET%^", environment(), ({ this_object(), living }));

            living->add_hostile(this_object());
            add_hostile(living);
        }
    }
}