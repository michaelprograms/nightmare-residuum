#include "living.h"

nosave private object *__Hostiles;

void create () {
    __Hostiles = ({ });
}

protected void handle_combat () {
    object target;
    mixed *weapons;
    int min, max, hits;

    if (time() % 2 || !(target = query_target_hostile())) return;
    if (this_object()->query_disable()) return;

    if (this_object()->query_posture() == "meditating") {
        this_object()->set_posture("sitting");
    }
    if (member_array(this_object()->query_posture(), ({ "sitting", "laying" })) > -1) {
        message("combat info", "You cannot fight while " + this_object()->query_posture() + " down!", this_object());
        return;
    }

    if ((this_object()->is_npc() || this_object()->is_monster()) && this_object()->query_ability_chance()) {
        this_object()->handle_ability_attack();
    }

    weapons = query_wielded_weapons() + query_wieldable_limbs();
    min = sizeof(weapons[0..2]) + query_stat("agility") / 100;
    max = sizeof(weapons[0..2]) + query_stat("agility") / 50;
    hits = min + secure_random(max - min + 1);

    if (!hits) {
        message("combat miss", this_object()->query_cap_name() + " " + element_of(({
            "flops about helplessly",
            "tries to look menacing",
            "uselessly flops around",
        })) + ".", environment(), this_object());
    }
    for (int h = 0; h < hits; h ++) {
        if (!target) break;
        handle_combat_hit(target, weapons[random(sizeof(weapons))]);
    }
    add_sp(-(secure_random(hits) + 1));

    if (target && !target->query_hostile(this_object())) {
        target->add_hostile(this_object());
    }
}

private void handle_combat_hit (object target, mixed weapon) {
    int hit = 0;
    string type, name, possessive, limb;

    if (objectp(weapon)) {
        type = weapon->query_type();
        name = weapon->query_name();
    } else if (stringp(weapon)) {
        type = "brawl";
        name = weapon;
    } else {
        return; // invalid weapon
    }

    // Miss chance
    hit = (query_stat("agility") * 75 / 100);
    hit += secure_random(query_stat("agility") * 75 / 100 + 1);
    hit += secure_random(target->query_stat("luck") * 5 / 100 + 1);
    hit += secure_random(query_skill(type + " attack") * 20 / 100 + 1);

    hit -= (target->query_stat("agility") * 25 / 100);
    hit -= secure_random(target->query_stat("agility") * 25 / 100 + 1);
    hit -= secure_random(target->query_stat("luck") * 5 / 100 + 1);
    hit -= secure_random(target->query_skill(type + " defense") * 20 / 100 + 1);

    possessive = possessive(this_object());
    limb = target->query_random_limb();

    if (hit < 1 || query_sp() < 1) {
        message("combat miss", "You miss " + target->query_cap_name() + " with your " + name + ".", this_object());
        message("combat miss", this_object()->query_cap_name() + " misses you with " + possessive + " " + name + ".", target);
        message("combat miss", this_object()->query_cap_name() + " misses " + target->query_cap_name() + " with " + possessive + " " + name + ".", environment(), ({ this_object(), target }));
        train_skill(type + " attack", 0.5);
        target->train_skill(type + " defense", 0.5);
    } else {
        int damage = 0;

        // Base Damage
        damage = secure_random(10);
        damage += (query_stat("strength") * 10 / 100);
        damage += secure_random(query_stat("strength") * 10 / 100 + 1);
        damage += secure_random(query_sp() * 10 / 100 + 1);
        damage += secure_random(query_stat("luck") * 5 / 100 + 1);
        damage += secure_random(query_skill(type + " attack") * 20 / 100 + 1);

        // apply target mitigations
        damage -= (query_stat("endurance") * 10 / 100);
        damage -= secure_random(query_stat("endurance") * 10 / 100 + 1);
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
}

varargs void check_lifesigns (object source) {
    int dead = 0;

    if (query_hp() < 1) {
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
    __Hostiles = filter_array(__Hostiles, (: !undefinedp($1) :));
    return __Hostiles;
}
object *query_present_hostiles () {
    return filter_array(query_hostiles(), (: environment($1) == environment() :));
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