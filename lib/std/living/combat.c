// @this_object /std/living.c

nosave private object *__Hostiles;

void create () {
    __Hostiles = ({ });
}

/* ----- hostiles ----- */

/**
 * Attempt to add a living object to this living object's hostile list.
 *
 * @param {STD_LIVING} ob the living object that is hostile
 * @returns 0 or 1 for success
 */
int add_hostile (object ob) {
    if (!ob || !ob->is_living() || member_array(ob, __Hostiles) > -1) {
        return 0;
    }
    if (ob == this_object() || ob->query_defeated() || this_object()->query_defeated()) {
        return 0;
    }
    __Hostiles += ({ ob });
    return 1;
}
/**
 * Remove a living object as a hostile to this living object.
 * @param {STD_LIVING} ob the living object to remove
 * @returns 0 or 1 for success
 */
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
/**
 * Returns a list of all living items that are currently hostile to this living
 * object.
 *
 * @returns {STD_LIVING*} the list of hostiles
 */
object *query_hostiles () {
    __Hostiles = filter(__Hostiles, (: !undefinedp($1) :));
    return __Hostiles;
}

/* ----- combat ----- */

void handle_combat_miss () {
    object to = this_object(), env = environment(this_object());
    string mymsg, yourmsg;
    switch(random(3)) {
    case 0:
        mymsg = "You flop about helplessly.";
        yourmsg = to->query_cap_name()+" flops about helplessly.";
        break;
    case 1:
        mymsg = "You try to look menacing.";
        yourmsg = to->query_cap_name()+" tries to look menacing.";
        break;
    case 2:
        mymsg = "You uselessly dance around.";
        yourmsg = to->query_cap_name()+" uselessly dances around.";
        break;
    }
    message("combat miss", mymsg, to);
    message("combat miss", yourmsg, env, to);
}
/**
 * An individual combat hit by this living object to another living target.
 *
 * @param {STD_LIVING} target the living object to hit
 * @param table the combat table for hit/miss/etc chances
 * @param {STD_WEAPON} weapon the wielded weapon, if any
 */
void handle_combat_hit (object target, mapping *table, object weapon) {
    object to = this_object();
    int d100, sum, crit, damage;
    string limb;
    if (!target) {
        return;
    }
    if (to->query_sp() > 0) {
        d100 = roll_die(1, 100)[0];
        sum = 0;
    }
    foreach (mapping m in table) {
        if (!m["value"]) {
            continue;
        }
        damage = 0;
        crit = 0;
        sum = min(({ 100, sum + m["value"], }));
        if (d100 <= sum) {
            switch (m["id"]) {
            case "miss":
                combat_miss_message(to, target, weapon);
                break;
            // case "resist": // @TODO
            //     break;
            case "block":
                combat_block_message(to, target);
                break;
            case "parry":
                combat_parry_message(to, target, weapon);
                break;
            case "evade":
                combat_evade_message(to, target);
                break;
            case "critical hit":
                crit = 1;
            case "regular hit":
                limb = target->query_random_limb();
                damage = combat_hit_damage(to, target, limb, weapon, crit);
                combat_hit_message(to, target, limb, weapon, damage, crit, 0);
                target->handle_damage(damage, limb);
                break;
            }
            break;
        }
    }
}
/**
 * The main combat handler, this will be called by the living object's
 * heartbeat.
 */
void handle_combat () {
    object to = this_object(), env = environment(), target, *weapons;
    int base, min, max, hits;

    target = present_hostile(to);
    to->check_lifesigns(target);
    if (!to || to->query_defeated() || to->query_disable() || !target || env != environment(target)) {
        return;
    }

    if (to->query_posture() == "meditating") {
        to->set_posture("sitting");
    }
    if (member_array(to->query_posture(), ({ "sitting", "laying" })) > -1) {
        message("combat info", "You cannot fight while " + to->query_posture() + " down!", to);
        return;
    }
    target->add_hostile(to);

    if (to->is_npc() && to->query_ability_chance()) {
        to->handle_ability_attack();
    }

    weapons = to->query_wielded_weapons() + to->query_wieldable_limbs();
    base = sizeof(weapons[0..2]) + to->query_stat("agility");
    min = base / 100;
    max = base / 50;
    hits = min + random(max - min + 1);

    if (!hits) {
        handle_combat_miss();
        return;
    }
    for (int h = 0; h < hits; h ++) {
        if (!target) {
            break;
        }
        handle_combat_hit(target, combat_table(to, target, h), element_of(weapons));
    }
    to->add_sp(-(random(hits) + 1));
    if (target) {
        target->check_lifesigns(to);
    }
}

/**
 * Check if this living object has been defeated.
 *
 * @param {STD_LIVING} source the optional living object requesting this check
 */
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