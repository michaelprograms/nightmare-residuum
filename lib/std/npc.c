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