inherit STD_LIVING;

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

    update_vitals(1);
}