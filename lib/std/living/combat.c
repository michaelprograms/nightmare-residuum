#include "living.h"

nosave private object *__Hostiles;

void create () {
    __Hostiles = ({});
}

protected void handle_combat () {
    object *targets, target;
    mixed *weapons;
    int min, max, hits;

    if (time() % 2) return;
    if (!sizeof(targets = query_present_hostiles())) return;

    target = targets[0];
    if (!target->query_hostile(this_object())) target->add_hostile(this_object());

    weapons = query_wielded_weapons() + query_wieldable_limbs();
    min = sizeof(weapons) + query_stat("agility") / 100;
    max = 1 + query_stat("agility") / 50;
    hits = min + secure_random(max - min + 1);

    for (int h = 0; h < hits; h ++) {
        handle_combat_hit(target, weapons[random(sizeof(weapons))]);
    }
    add_sp(-(secure_random(hits) + 1));
    target->check_lifesigns(this_object());
}

private void handle_combat_hit (object target, mixed weapon) {
    int hit = 0;
    string type, name, possessive;

    if (objectp(weapon)) {
        type = weapon->query_type();
        name = weapon->query_name();
    } else if (stringp(weapon)) {
        type = "melee";
        name = weapon;
    } else {
        return; // invalid weapon
    }

    // Miss chance
    hit = query_stat("agility") * 25 / 100;
    hit += secure_random(query_stat("agility") * 75 / 100 + 1);
    hit += secure_random(target->query_stat("luck") * 5 / 100 + 1);
    hit += secure_random(query_skill(type + " attack") * 20 / 100 + 1);

    hit -= target->query_stat("agility") * 75 / 100;
    hit -= secure_random(target->query_stat("agility") * 25 / 100 + 1);
    hit -= secure_random(target->query_stat("luck") * 10 / 100 + 1);
    hit -= secure_random(target->query_skill(type + " defense") * 20 / 100 + 1);

    possessive = possessive(this_object());

    if (hit < 1 || query_sp() < 1) {
        message("combat miss", "You miss " + target->query_name() + " with your " + name + ".\n", this_object());
        message("combat miss", this_object()->query_name() + " misses you with " + possessive + " " + name + ".\n", target);
        message("combat miss", this_object()->query_name() + " misses " + target->query_name() + " with " + possessive + " " + name + ".\n", environment(this_object()), ({ this_object(), target }));
        train_skill(type + " attack", 0.5);
        target->train_skill(type + " defense", 0.5);
    } else {
        int damage = 0;

        // Base Damage
        damage = query_stat("strength") * 10 / 100;
        damage += secure_random(query_stat("strength") * 10 / 100 + 1);
        damage += secure_random(query_sp() * 10 / 100 + 1);
        damage += secure_random(query_stat("luck") * 5 / 100 + 1);
        damage += secure_random(query_skill(type + " attack") * 20 / 100 + 1);

        // apply target mitigations
        damage -= query_stat("endurance") * 10 / 100;
        damage -= secure_random(query_stat("endurance") * 10 / 100 + 1);
        damage -= secure_random(query_hp() * 10 / 100 + 1);
        damage -= secure_random(query_stat("luck") * 10 / 100 + 1);
        damage -= secure_random(query_skill(type + " defense") * 20 / 100 + 1);

        if (damage < 1) {
            message("combat hit", "You hit " + target->query_name() + " ineffectively with your " + name + "\n", this_object());
            message("combat hit", this_object()->query_name() + " hits you ineffectively with " + possessive + " " + name + ".\n", target);
            message("combat hit", this_object()->query_name() + " hits " + target->query_name() + " ineffectively with " + possessive + " " + name + ".\n", environment(this_object()), ({ this_object(), target }));
        } else {
            // @TODO messages based upon type
            message("combat hit", "You hit " + target->query_name() + " with your " + name + ".\n", this_object());
            message("combat hit", this_object()->query_name() + " hits you with " + possessive + " " +name + ".\n", target);
            message("combat hit", this_object()->query_name() + " hits " + target->query_name() + " with " + possessive + " " + name +".\n", environment(this_object()), ({ this_object(), target }));
            target->handle_damage(damage, this_object());
        }
        train_skill(type + " attack");
        target->train_skill(type + " defense");
    }
}

varargs void check_lifesigns (object source) {
    if (query_hp() < 1) {
        message("system", "\nYou have been %^BOLD%^RED%^defeated%^RESET%^!\n\n", this_object());
        message("system", "\n" + this_object()->query_name() + " has been %^BOLD%^RED%^defeated%^RESET%^!\n\n", environment(this_object()), this_object());
        if (source) source->handle_victory(this_object());
        handle_defeat(this_object()->is_character());
    }
}

varargs int handle_damage (int damage, object source) {
    add_hp(-damage);
    if (query_max_hp() < query_hp()) set_hp(query_max_hp());
    if (this_object()->is_character()) {
        message("system", sprintf("hp: %d    sp: %d    mp: %d\n", query_hp(), query_sp(), query_mp()), this_object());
    }
    return damage;
}

/* ----- hostiles ----- */

int add_hostile (object ob) {
    if (!ob || !ob->is_living() || member_array(ob, __Hostiles) > -1) {
        return 0;
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
    return filter_array(query_hostiles(), (: environment($1) == environment(this_object()) :));
}

/* ----- parser applies ----- */

mixed direct_attack_liv () {
    object ob = previous_object();
    return environment() == environment(ob) && ob != this_object() && !query_hostile(ob);
}