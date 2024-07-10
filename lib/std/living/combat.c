nosave private object *__Hostiles;

void create () {
    __Hostiles = ({ });
}

/* ----- hostiles ----- */

int add_hostile (object ob) {
    if (!ob || !ob->is_living() || member_array(ob, __Hostiles) > -1) {
        return 0;
    }
    if (ob->query_defeated() || this_object()->query_defeated()) {
        return 0;
    }
    if (ob == this_object()) {
        error("Bad argument 1 to combat->add_hostile");
    }
    __Hostiles += ({ ob });
    return 1;
}
int remove_hostile (object ob) {
    if (!ob || !ob->is_living() || member_array(ob, __Hostiles) == -1) {
        return 0;
    }
    __Hostiles -= ({ ob });
    return 1;
}
int query_hostile (object ob) {
    return member_array(ob, __Hostiles) > -1;
}
object *query_hostiles () {
    __Hostiles = filter(__Hostiles, (: !undefinedp($1) :));
    return __Hostiles;
}
object *query_present_hostiles () {
    return filter(query_hostiles(), (: environment($1) == environment() :));
}
object query_target_hostile () {
    object *hostiles = query_present_hostiles();
    return sizeof(hostiles) ? hostiles[0] : 0;
}

/* ----- combat table ----- */

mapping *query_combat_table (object target, int hit) {
    mapping *table;
    float levelAdjust;

    levelAdjust = (target->query_level() - this_object()->query_level()) / 5.0;

    table = ({
        ([
            "id": "miss",
            "value": min(({ 0.0, 5.0 + levelAdjust + hit }))
        ]),
        ([
            "id": "resist",
            "value": 0.0
        ]),
        ([
            "id": "block",
            "value": (target && target->query_worn_shield() ? min(({ 0.0, 5.0 + levelAdjust - hit })) : 0.0)
        ]),
        ([
            "id": "parry",
            "value": (sizeof(target && target->query_wielded_weapons()) ? min(({ 0.0, 5.0 + levelAdjust - hit })) : 0.0)
        ]),
        ([
            "id": "evade",
            "value": min(({ 0.0, 5.0 + levelAdjust - hit }))
        ]),
        ([
            "id": "critical hit",
            "value": min(({ 0.0, 5.0 + levelAdjust - hit }))
        ]),
        ([
            "id": "regular hit",
            "value": 100.0
        ]),
    });

    return table;
}

/* ----- combat ----- */

private void handle_combat_miss (object target, mixed weapon) {
    string type, name, possessive = possessive(this_object());

    if (objectp(weapon)) {
        type = weapon->query_type();
        name = weapon->query_name();
    } else if (stringp(weapon)) {
        type = "brawl";
        name = weapon;
    } else {
        error("Bad argument 2 to combat->handle_combat_miss");
    }

    message("combat miss", "You miss " + target->query_cap_name() + " with your " + name + ".", this_object());
    message("combat miss", this_object()->query_cap_name() + " misses you with " + possessive + " " + name + ".", target);
    message("combat miss", this_object()->query_cap_name() + " misses " + target->query_cap_name() + " with " + possessive + " " + name + ".", environment(), ({ this_object(), target }));
}
private void handle_combat_block (object target) {
    string possessive = possessive(this_object());
    object shield = this_object()->query_worn_shield();

    if (!shield) {
        error("Bad argument 2 to combat->handle_combat_block");
    }

    message("combat miss", "You block " + target->query_cap_name() + " with your " + shield->query_name() + ".", this_object());
    message("combat miss", this_object()->query_cap_name() + " blocks you with " + possessive + " " + shield->query_name() + ".", target);
    message("combat miss", this_object()->query_cap_name() + " blocks " + target->query_cap_name() + " with " + possessive + " " + shield->query_name() + ".", environment(), ({ this_object(), target }));
}
private void handle_combat_parry (object target) {
    string type, name, possessive = possessive(this_object());
    mixed *weapons = this_object()->query_wielded_weapons() + this_object()->query_wieldable_limbs(), weapon = element_of(weapons);

    if (objectp(weapon)) {
        type = weapon->query_type();
        name = weapon->query_name();
    } else if (stringp(weapon)) {
        type = "brawl";
        name = weapon;
    } else {
        error("Bad argument 2 to combat->handle_combat_parry");
    }

    message("combat miss", "You parry " + target->query_cap_name() + " with your " + name + ".", this_object());
    message("combat miss", this_object()->query_cap_name() + " parries you with " + possessive + " " + name + ".", target);
    message("combat miss", this_object()->query_cap_name() + " parries " + target->query_cap_name() + " with " + possessive + " " + name + ".", environment(), ({ this_object(), target }));
}
private void handle_combat_evade (object target) {
    message("combat miss", "You evade " + possessive_noun(target->query_cap_name()) + " attack.", this_object());
    message("combat miss", this_object()->query_cap_name() + " evades your attack.", target);
    message("combat miss", this_object()->query_cap_name() + " evades " + possessive_noun(target->query_cap_name()) + " attack.", environment(), ({ this_object(), target }));
}
private void handle_combat_hit (object target, mixed weapon, int crit) {
    int dice, damage = 0;
    string type, name, limb;

    if (objectp(weapon)) {
        type = weapon->query_type();
        name = weapon->query_name();
    } else if (stringp(weapon)) {
        type = "brawl";
        name = weapon;
    } else {
        return; // invalid weapon
    }

    limb = target->query_random_limb();

    // Base Damage
    damage += roll_die(1, 6)[0];

    dice = max(({ 1, random(this_object()->query_stat("strength") + 1) * 5 / 100 }));
    damage += roll_die(dice, 6)[0];

    dice = max(({ 1, random(this_object()->query_stat("luck") + 1) * 5 / 100 }));
    damage += roll_die(dice, 6)[0];

    if (crit) {
        damage = damage * 3 / 2;
    }

    // apply target mitigations
    dice = max(({ 1, random(target->query_stat("endurance") + 1) * 10 / 100 }));
    damage -= roll_die(dice, 6)[0];

    dice = max(({ 1, random(target->query_stat("luck") + 1) * 10 / 100 }));
    damage -= roll_die(dice, 6)[0];

    damage -= target->query_limb_armor(limb);
    damage -= target->query_protection();

    display_combat_message(this_object(), target, limb, weapon, type, damage, crit, 0);
    if (damage > 0) {
        target->handle_damage(damage, limb, this_object());
    }
}

protected void handle_combat () {
    object target, *weapons;
    int min, max, hits;
    int d100;
    float sum = 0;

    target = query_target_hostile();
    this_object()->check_lifesigns(query_target_hostile());
    if (
        !this_object() ||
        this_object()->query_defeated() ||
        this_object()->query_disable() ||
        !target ||
        environment() != environment(target)
    ) {
        return;
    }

    if (this_object()->query_posture() == "meditating") {
        this_object()->set_posture("sitting");
    }
    if (member_array(this_object()->query_posture(), ({ "sitting", "laying" })) > -1) {
        message("combat info", "You cannot fight while " + this_object()->query_posture() + " down!", this_object());
        return;
    }

    target->add_hostile(this_object());

    if (this_object()->is_npc() && this_object()->query_ability_chance()) {
        this_object()->handle_ability_attack();
    }

    weapons = this_object()->query_wielded_weapons() + this_object()->query_wieldable_limbs();

    min = sizeof(weapons[0..2]) + this_object()->query_stat("agility") / 100;
    max = sizeof(weapons[0..2]) + this_object()->query_stat("agility") / 50;
    hits = min + random(max - min + 1);

    if (!hits) {
        message("combat miss", this_object()->query_cap_name() + " " + element_of(({
            "flops about helplessly",
            "tries to look menacing",
            "uselessly flops around",
        })) + ".", environment(), this_object());
    }

    for (int h = 0; h < hits; h ++) {
        if (!target) {
            break;
        }

        if (this_object()->query_sp() > 0) {
            d100 = roll_die(1, 100)[0];
        }
        sum = 0;
        foreach (mapping m in query_combat_table(target, h)) {
            if (!m["value"]) {
                continue;
            }
            sum = min(({ 100.0, sum + m["value"], }));
            if (d100 <= sum) {
                switch (m["id"]) {
                case "miss":
                    handle_combat_miss(target, element_of(weapons));
                    break;
                // case "resist": // @TODO
                //     break;
                case "block":
                    target->handle_combat_block(target);
                    break;
                case "parry":
                    target->handle_combat_parry(target, element_of(weapons));
                    break;
                case "evade":
                    target->handle_combat_evade(target);
                    break;
                case "critical hit":
                    handle_combat_hit(target, weapons[random(sizeof(weapons))], 1);
                    break;
                case "regular hit":
                    handle_combat_hit(target, weapons[random(sizeof(weapons))], 0);
                    break;
                }
                break;
            }
        }

    }
    this_object()->add_sp(-(random(hits) + 1));
    if (target) {
        target->check_lifesigns(this_object());
    }
}

varargs void check_lifesigns (object source) {
    int dead = 0;

    if (this_object()->query_defeated()) {
        return;
    }

    if (this_object()->query_hp() < 0) {
        dead = 1;
    }
    if (!dead) {
        foreach (string limb in this_object()->query_severed_limbs()) {
            if (this_object()->query_limb(limb)["type"] == "FATAL") {
                dead = 1;
            }
        }
    }
    if (dead) {
        message("defeat", "\nYou have been %^I_RED%^BOLD%^defeated%^RESET%^!\n", this_object());
        message("defeat", "\n" + this_object()->query_cap_name() + " has been %^I_RED%^BOLD%^defeated%^RESET%^!\n", environment(), this_object());
        if (source) {
            source->handle_victory(this_object());
        }
        this_object()->handle_defeat(source);
    }
}

/* ----- parser applies ----- */

mixed direct_attack_liv (mixed args...) {
    object po = previous_object();
    return environment() == environment(po) && po != this_object() && !query_hostile(po);
}
mixed direct_aid_liv (mixed args...) {
    return direct_attack_liv(args);
}