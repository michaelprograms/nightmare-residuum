#include "living.h"

nosave private object *__Hostiles;

void create () {
    __Hostiles = ({ });
}

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

protected void handle_combat () {
    object target, *weapons;
    int min, max, hits;
    mapping *table;
    int d100;
    float sum = 0;

    target = query_target_hostile();
    this_object()->check_lifesigns(query_target_hostile());
    if (!this_object() || this_object()->query_defeated() || this_object()->query_disable()) {
        return;
    }
    if (!target || environment() != environment(target) || time() % 2) {
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

    weapons = query_wielded_weapons() + query_wieldable_limbs();

    min = sizeof(weapons[0..2]) + query_stat("agility") / 100;
    max = sizeof(weapons[0..2]) + query_stat("agility") / 50;
    hits = min + random(max - min + 1);

    if (!hits) {
        message("combat miss", this_object()->query_cap_name() + " " + element_of(({
            "flops about helplessly",
            "tries to look menacing",
            "uselessly flops around",
        })) + ".", environment(), this_object());
    }

    for (int h = 0; h < hits; h ++) {
        if (!target) break;

        table = query_combat_table(target, h);

        if (query_sp() > 0) {
            d100 = roll_die(1, 100)[0];
        }
        sum = 0;
        foreach (mapping m in table) {
            if (!m["value"]) continue;
            sum = min(({ 100.0, sum + m["value"], }));
            if (d100 <= sum) {
                switch(m["id"]) {
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
    add_sp(-(random(hits) + 1));
    if (target) {
        target->check_lifesigns();
    }
}

private void handle_combat_miss (object target, mixed weapon) {
    string type, name, possessive = possessive(this_object());

    if (objectp(weapon)) {
        type = weapon->query_type();
        name = weapon->query_name();
    } else if (stringp(weapon)) {
        type = "brawl";
        name = weapon;
    } else {
        return; // invalid weapon
    }

    message("combat miss", "You miss " + target->query_cap_name() + " with your " + name + ".", this_object());
    message("combat miss", this_object()->query_cap_name() + " misses you with " + possessive + " " + name + ".", target);
    message("combat miss", this_object()->query_cap_name() + " misses " + target->query_cap_name() + " with " + possessive + " " + name + ".", environment(), ({ this_object(), target }));
    train_skill(type + " attack", 0.5);
    target->train_skill(type + " defense", 0.5);
}
private void handle_combat_block (object target) {
    string possessive = possessive(this_object());
    object shield = this_object()->query_worn_shield();

    if (!shield) {
        return; // invalid shield
    }

    message("combat miss", "You block " + target->query_cap_name() + " with your " + shield->query_name() + ".", this_object());
    message("combat miss", this_object()->query_cap_name() + " blocks you with " + possessive + " " + shield->query_name() + ".", target);
    message("combat miss", this_object()->query_cap_name() + " blocks " + target->query_cap_name() + " with " + possessive + " " + shield->query_name() + ".", environment(), ({ this_object(), target }));
}
private void handle_combat_parry (object target) {
    string type, name, possessive = possessive(this_object());
    mixed *weapons = query_wielded_weapons() + query_wieldable_limbs(), weapon = element_of(weapons);

    if (objectp(weapon)) {
        type = weapon->query_type();
        name = weapon->query_name();
    } else if (stringp(weapon)) {
        type = "brawl";
        name = weapon;
    } else {
        return; // invalid weapon
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
    int damage = 0;
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
    damage = random(10);
    damage += (query_stat("strength") * 10 / 100);
    damage += random(query_stat("strength") * 10 / 100 + 1);
    damage += random(query_sp() * 10 / 100 + 1);
    damage += random(query_stat("luck") * 5 / 100 + 1);
    damage += random(query_skill(type + " attack") * 20 / 100 + 1);
    if (crit) {
        damage = damage * 3 / 2;
    }

    // apply target mitigations
    damage -= (query_stat("endurance") * 10 / 100);
    damage -= random(query_stat("endurance") * 10 / 100 + 1);
    damage -= secure_random(query_hp() * 10 / 100 + 1);
    damage -= secure_random(query_stat("luck") * 10 / 100 + 1);
    damage -= secure_random(query_skill(type + " defense") * 20 / 100 + 1);
    damage -= target->query_limb_armor(limb);
    damage -= target->query_protection();

    display_combat_message(this_object(), target, limb, weapon, type, damage, 0);
    if (damage > 0) target->handle_damage(damage, limb, this_object());

    train_skill(type + " attack");
    if (target) target->train_skill(type + " defense");
}

varargs void check_lifesigns (object source) {
    int dead = 0;

    if (this_object()->query_defeated()) {
        return;
    }

    if (query_hp() < 0) {
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
        message("defeat", "\nYou have been %^BOLD%^RED%^defeated%^RESET%^!\n", this_object());
        message("defeat", "\n" + this_object()->query_cap_name() + " has been %^BOLD%^RED%^defeated%^RESET%^!\n", environment(), this_object());
        if (source) source->handle_victory(this_object());
        handle_defeat(source);
    }
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

/* ----- parser applies ----- */

mixed direct_attack_liv (mixed args...) {
    object po = previous_object();
    return environment() == environment(po) && po != this_object() && !query_hostile(po);
}
mixed direct_aid_liv (mixed args...) {
    return direct_attack_liv(args);
}