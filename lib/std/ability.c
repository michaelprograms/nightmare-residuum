// @this_object /std/ability.c

#include <verb.h>

inherit STD_VERB;
inherit "/std/ability/config.c";
inherit "/std/ability/cost.c";
inherit "/std/ability/helpers.c";
inherit "/std/ability/message.c";
inherit "/std/ability/requirements.c";
inherit "/std/ability/target.c";
inherit "/std/ability/weapons.c";

nosave private int __Cooldown = 1;

/* ----- ability cooldown ----- */

void set_cooldown (int n) {
    if (undefinedp(n) || !intp(n) || n < 0) {
        error("Bad argument 1 to ability->set_cooldown");
    }
    __Cooldown = n;
}
int query_cooldown () {
    return __Cooldown;
}

/* ----- calculations ----- */

/**
 * Determine how much source will heal target.
 *
 * @param {STD_LIVING} source the source of the heal
 * @param {STD_LIVING} target the target of the heal
 * @param limb the targeted limb if any
 * @returns integer amount of heal
 */
int calculate_heal (object source, object target, string limb) {
    int damage;
    int sourceStat, targetStat;
    int n;

    // base heal
    damage = 1 + random(5 + source->query_level());
    damage += random(source->query_stat("luck") * 5 / 100 + 1);

    // skill powers
    // @TODO: skills were removed
    foreach (string key,int value in query_powers()) {
        switch (key) {
        case "theurgy":
            n = 1;
            break;
        case "medicine": default:
            n = 2;
            break;
        }
        sourceStat = source->query_stat("intelligence");
        damage += sourceStat * n / 4 + random(sourceStat * (4-n) / 4);
    }

    targetStat = target->query_stat("charisma");
    damage += ((targetStat * 10 / 100) + random(targetStat * 10 / 100 + 1));

    return damage;
}

/**
 * Determine by how much the source will damage the target.
 *
 * @param {STD_LIVING} source the source of the damage
 * @param {STD_LIVING} target the target of the damage
 * @param limb the targeted limb if any
 * @returns integer amount of damage
 */
int calculate_damage (object source, object target, string limb) {
    int dice, damage, tmp;
    int sourceDieSides = query_die_sides(source);
    int targetDieSides = query_die_sides(target);

    // level damage
    dice = max(({ 1, source->query_level() * 10 / 100 }));
    damage += roll_die(dice, sourceDieSides)[0];

    dice = max(({ 1, random(source->query_stat("luck") + 1) * 4 / 100 }));
    damage += roll_die(dice, sourceDieSides)[0];

    foreach (string key, int value in query_powers()) {
        switch (key) {
            case "psionic":
                damage += roll_die(1, value)[0];
                tmp = source->query_stat("intelligence");
                break;
            case "ranged":
                damage += roll_die(1, value)[0];
                tmp = source->query_stat("agility");
                break;
            case "brawl": default:
                damage += roll_die(1, value)[0];
                tmp = source->query_stat("strength");
                break;
        }
        // stat damage
        dice = max(({ 1, tmp * 10 / 100 }));
        damage += roll_die(dice, sourceDieSides)[0];
    }

    // apply target mitigations
    dice = max(({ 1, target->query_level() / 5 }));
    damage -= roll_die(dice, targetDieSides)[0];

    dice = max(({ 1, target->query_stat("endurance") / 10 }));
    damage -= roll_die(dice, targetDieSides)[0];

    dice = max(({ 1, random(target->query_stat("luck") + 1) / 25 }));
    damage -= roll_die(dice, targetDieSides)[0];

    damage -= target->query_limb_armor(limb);
    damage -= target->query_protection();

    if (damage < 0) {
        damage = 0;
    }

    return damage;
}

/* ----- difficulty factor ----- */

nosave private int __DifficultyFactor;

void set_difficulty_factor (int factor) {
    if (undefinedp(factor) || !intp(factor)) {
        error("Bad argument 1 to ability->set_difficulty_factor");
    }
    __DifficultyFactor = factor;
}
int query_difficulty_factor () {
    if (!__DifficultyFactor) {
        __DifficultyFactor = 100;
    }
    return __DifficultyFactor;
}

/* ----- targets ----- */

object *verify_targets (object source, object *targets) {
    if (query_type() == "attack") {
        if ((!targets || !targets[0])) {
            targets = ({ present_hostile(source) });
        }
        if ((!targets || !targets[0])) {
            return 0;
        }
    } else if (query_type() == "heal" || query_type() == "utility") {
        if ((!targets || !targets[0])) {
            targets = ({ source });
        }
        if ((!targets || !targets[0])) {
            return 0;
        }
    }
    return targets;
}

/* ----- utility ----- */

void handle_utility (object source, object target, string limb) {
    // @TODO: refactor this to be std/ability inheritable and more automatic
    // override this function for utility abilities
}

/* ----- success ----- */

/**
 * Check if the usage of an ability will succeed or not.
 *
 * @param {STD_LIVING} source the source of the ability
 * @param {STD_LIVING} target the target of the ability
 * @returns 0 or 1 for success
 */
int is_ability_successful (object source, object target) {
    int sourceN = 0, targetN = 0;
    int chance = 100;
    int powerTotal = query_total_skill_power();

    // @TODO if (target->query_paralyzed()) return 100;

    if (query_type() == "attack") {
        foreach (string key,int value in query_powers()) {
            if (key == "psionic") {
                sourceN += source->query_stat("intelligence") * value / powerTotal;
                targetN += target->query_stat("perception") * value / powerTotal;
            } else if (key == "ranged") {
                sourceN += source->query_stat("agility") * value / powerTotal;
                targetN += target->query_stat("luck") * value / powerTotal;
            } else { /* blade, blunt, brawl */
                sourceN += source->query_stat("strength") * value / powerTotal;
                targetN += target->query_stat("endurance") * value / powerTotal;
            }
        }
        // success range is 10% to 100%
        targetN = targetN * query_difficulty_factor() / 100;
        if (sourceN < targetN) {
            chance = sourceN * 100 / targetN;
            chance = max(({ 10, min(({ 100, chance })) }));
        }
        return (1+random(100)) <= chance;
    } else if (query_type() == "heal" || query_type() == "utility") {
        return 1;
    }
    return 0;
}

/* ----- use ability ----- */

/**
 * Handle a living object using an ability, determining requirements,
 * success or failure, damage, and messaging.
 *
 * @param {STD_LIVING} source the source of the ability use
 * @param {STD_LIVING*} targets the target(s) of the ability use
 */
private void handle_ability_use (object source, object *targets) {
    mapping cost;
    int damage;
    /** @type {STD_WEAPON} weapon */
    object weapon;
    string limb;
    int l;

    if (!query_type()) {
        error("Ability "+query_name()+" does not have an ability type set");
    }

    if (!verify_ability_requirements(source)) {
        message("action", "You cannot do that.", source);
        return;
    }

    if (source->query_cooldown(query_name()) > 0) {
        message("action", "You are not yet ready to " + query_name() + " again.", source);
        return;
    }

    if (!(targets = verify_targets(source, targets))) {
        message("action", "You have no " + (query_type() == "attack" ? "hostile" : "friendly") + " targets present.", source);
        return;
    }

    if (__Weapons["brawl"]) {
        if (sizeof(source->query_wieldable_limbs()) < __Weapons["brawl"][0]) {
            message("action", "You do not have any free hands.", source);
            return;
        }
    } else if (sizeof(__Weapons) && !(weapon = query_best_weapon(source))) {
        message("action", "You are not wielding the correct type of weapon.", source);
        return;
    }

    if (query_targets() < sizeof(targets)) {
        return;
    }

    l = min(({ sizeof(targets), query_targets() }));

    // determine cost
    cost = query_cost();
    if (!verify_cost(source, cost)) {
        return;
    }
    apply_cost(source, cost);

    // update statuses
    source->set_busy(1);
    // @TODO re-enable this when determing busy vs disable
    // source->set_disable(1);

    if (__Cooldown > 0) {
        source->set_cooldown(query_name(), __Cooldown);
    }

    // send attempt messages
    this_object()->ability_message_attempt(source, targets);

    foreach (object target in targets) {
        if (query_type() == "attack") {
            if (!source->query_hostile(target)) {
                initiate_combat(source, target);
            }
        }
        limb = target->query_random_limb();
        // send success or fail messages
        if (is_ability_successful(source, target)) {
            this_object()->ability_message_success(source, target, limb);

            if (query_type() == "attack") {
                // determine damage
                damage = calculate_damage(source, target, limb);
                combat_hit_message(source, target, limb, query_name(), damage, 0, 1);
                target->handle_damage(damage, limb);

                ability_debug_message(source, target, damage);
            } else if (query_type() == "heal") {
                // determine heal
                damage = calculate_heal(source, target, limb);
                combat_heal_message(source, target, limb, damage);
                // @TODO determine hp/sp/mp
                target->add_hp(damage);

                ability_debug_message(source, target, damage);
            } else if (query_type() == "utility") {
                this_object()->handle_utility(source, target, limb);
            }
        } else {
            this_object()->ability_message_fail(source, target, limb);
        }
    }
}

/* ----- help ----- */

string handle_help (object char) {
    string result, *tmp;
    int n;

    result = ::handle_help(char);
    result += "\n%^I_CYAN%^BOLD%^Type%^RESET%^\n" + capitalize(query_type()) + "\n";
    if (sizeof(__Reqs)) {
        foreach (string key,mapping value in __Reqs) {
            result += "\n%^I_CYAN%^BOLD%^" + capitalize(key) + " Requirements%^RESET%^\n";
            tmp = ({ });
            if (!undefinedp(value["level"]) && intp(value["level"])) {
                tmp += ({ "Level " + value["level"] });
            }
            if (mapp(value["stats"])) {
                foreach (string stat,int num in value["stats"]) {
                    tmp += ({ num + " " + stat });
                }
            }
            if (mapp(value["skills"])) {
                foreach (string skill,int num in value["skills"]) {
                    tmp += ({ num + " " + skill });
                }
            }
            result += implode(tmp, ", ") + "\n";
        }
    }
    if (__Cooldown != 1) {
        result += "\n%^I_CYAN%^BOLD%^Cooldown%^RESET%^\n";
        result += __Cooldown + " rounds\n";
    }
    if (n = sizeof(__Weapons)) {
        result += "\n%^I_CYAN%^BOLD%^Weapons%^RESET%^\n";
        foreach (string key,int *value in __Weapons) {
            result += implode(map(value, (: cardinal($1)+" handed "+$(key) :)), ", ") + "\n";
        }
    }
    return result;
}

/* ----- applies ----- */

int direct_verb_liv (mixed args...) {
    /** @type {STD_LIVING} living */
    object living;

    if (sizeof(args) < 2) {
        return 0;
    }

    living = args[1];
    if (!livingp(living)) {
        return 0;
    }
    if (query_type() == "attack") {
        if (living == previous_object() || living->query_defeated()) {
            return 0;
        }
        return 1;
    } else if (query_type() == "heal" || query_type() == "utility") {
        return 1;
    }
    return 0;
}

mixed can_verb_lvs (mixed args...) {
    return can_verb_rule(args);
}
mixed can_verb_liv (mixed args...) {
    return can_verb_rule(args);
}
mixed can_verb (mixed args...) {
    return can_verb_rule(args);
}

// Handle multiple target input
void do_verb_lvs (mixed args...) {
    object *targets;

    if (sizeof(args) < 2) {
        return;
    }

    targets = args[1];
    if (sizeof(targets) && query_targets() == 1) {
        handle_ability_use(previous_object(), ({ targets[0] }));
        return;
    }
    if (query_targets() > 1) {
        handle_ability_use(previous_object(), targets[0..query_targets()-1]);
    }
}

// Handle single target input
void do_verb_liv (mixed args...) {
    object target;

    // verify target
    if (arrayp(args)) {
        target = args[1];
    }

    handle_ability_use(previous_object(), ({ target }));
}

// Handle no input
void do_verb_rule (mixed args...) {
    handle_ability_use(previous_object(), ({ 0 }));
}

void create () {
    ::create();
    __Reqs = ([ ]);
    set_targets(1);
    if (query_name() != "ability") {
        add_rules(({ "", "LIV", "LVS", }));
    }
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax(query_name()+" ([target])");
}