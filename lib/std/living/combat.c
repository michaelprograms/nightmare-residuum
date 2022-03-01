#include "living.h"

nosave private object *__Hostiles;

void create () {
    __Hostiles = ({});
}

protected void handle_combat () {
    object *targets, target;
    int min, max, hits;

    if (time() % 2) return;
    if (!sizeof(targets = query_present_hostiles())) return;

    target = targets[0];
    min = 1 + query_stat("agility") / 100;
    max = 1 + query_stat("agility") / 50;
    hits = min + secure_random(max - min + 1);

    for (int h = 0; h < hits; h ++) {
        handle_combat_hit(target);
    }
}

private void handle_combat_hit (object target) {
    int hit = 0;
    int damage = 0;

    // Miss chance
    hit -= target->query_stat("agility") * 3 / 4 + secure_random(target->query_stat("luck") / 10 + 1);
    write("hit: "+hit+"\n");
    hit += query_stat("agility") / 2 - secure_random(query_stat("agility") / 2) + secure_random(target->query_stat("luck") / 20 + 1);
    write("hit: "+hit+"\n");
    if (hit < 1) {
        message("combat miss", "You miss " + target->query_name() + ".\n", this_object());
        message("combat miss", this_object()->query_name() + " misses you.\n", target);
        message("combat miss", this_object()->query_name() + " misses " + target->query_name() + ".\n", environment(this_object()), ({ this_object(), target }));
        return;
    }

    // Base Damage
    damage += query_stat("strength") / 10;
    damage += secure_random(query_stat("strength") / 10 + 1);
    damage += secure_random(query_sp() / 50 + 1);
    damage += secure_random(query_stat("luck") / 5 + 1);

    // apply target mitigations
    // @TODO

    if (damage < 1) {
        message("combat hit", "You hit " + target->query_name() + " ineffectively!\n", this_object());
        message("combat hit", this_object()->query_name() + " hits you ineffectively!\n", target);
        message("combat hit", this_object()->query_name() + " hits " + target->query_name() + " ineffectively!\n", environment(this_object()), ({ this_object(), target }));
    } else {
        message("combat hit", "You hit " + target->query_name() + "!\n", this_object());
        message("combat hit", this_object()->query_name() + " hits you!\n", target);
        message("combat hit", this_object()->query_name() + " hits " + target->query_name() + "!\n", environment(this_object()), ({ this_object(), target }));
    }

    handle_damage(damage);
}

int handle_damage (int damage) {
    add_hp(-damage);
    if (query_max_hp() < query_hp()) set_hp(query_max_hp());
    message("system", sprintf("hp: %d    sp: %d    mp: %d\n", query_hp(), damage, query_sp(), query_mp()), this_object());
    if (query_hp() < 1) {
        message("system", "\n%^BOLD%^RED%^You have perished!%^RESET%^\n\n", this_object());
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