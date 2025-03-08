// @this_object /std/ability.c

#include <verb.h>

inherit STD_VERB;
inherit "/std/ability/config.c";
inherit "/std/ability/cost.c";
inherit "/std/ability/requirements.c";
inherit "/std/ability/weapons.c";

nosave private int __NumTargets = 1;
nosave private int __Cooldown = 1;

/* ----- ability targets ----- */

void set_targets (int n) {
    if (undefinedp(n) || !intp(n) || n < 1) {
        error("Bad argument 1 to ability->set_targets");
    }
    __NumTargets = n;
}
int query_targets () {
    return __NumTargets;
}

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
    damage = random(5 + source->query_level());
    damage += random(source->query_stat("luck") * 5 / 100 + 1);

    // skill powers
    // @TODO: skills were removed
    foreach (string key,int value in query_powers()) {
        switch (key) {
        case "anatomy":
            n = 3;
            break;
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
 * Determine how much source will damage target.
 *
 * @param {STD_LIVING} source the source of the damage
 * @param {STD_LIVING} target the target of the damage
 * @param limb the targeted limb if any
 * @returns integer amount of damage
 */
int calculate_damage (object source, object target, string limb) {
    int dice, damage, tmp;
    int dieSides;

    if (source->query_class() == "psionist") {
        dieSides = 8;
    } else if (source->query_class() == "mystic") {
        dieSides = 7;
    } else {
        dieSides = 6;
    }

    // level damage
    dice = max(({ 1, source->query_level() * 10 / 100 }));
    damage += roll_die(dice, dieSides)[0];

    dice = max(({ 1, random(source->query_stat("luck") + 1) * 4 / 100 }));
    damage += roll_die(dice, dieSides)[0];

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
        damage += roll_die(dice, dieSides)[0];
    }

    if (target->query_class() == "psionist") {
        dieSides = 8;
    } else if (target->query_class() == "mystic") {
        dieSides = 7;
    } else {
        dieSides = 6;
    }

    // apply target mitigations
    dice = max(({ 1, target->query_level() / 5 }));
    damage -= roll_die(dice, dieSides)[0];

    dice = max(({ 1, target->query_stat("endurance") / 10 }));
    damage -= roll_die(dice, dieSides)[0];

    dice = max(({ 1, random(target->query_stat("luck") + 1) / 25 }));
    damage -= roll_die(dice, dieSides)[0];

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

/* ----- messages ----- */

/**
 * Display a message about attempting to use an ability.
 *
 * @param {STD_LIVING} source the source of the attempt
 * @param {STD_LIVING*} targets the target(s) of the attempt
 */
void ability_message_attempt (object source, object *targets) {
    string names;
    int n;

    // move source to end of list
    if ((n = member_array(source, targets)) > -1) {
        targets -= ({ source });
        targets += ({ source });
    }

    names = conjunction(map(targets, (: $1 == $(source) ? "yourself" : $1->query_cap_name() :)));

    if (query_type() == "attack") {
        message("action", "You attempt to " + query_name() + " " + names + "!", source);
        message("action", source->query_cap_name() + " attempts to " + query_name() + " you!", targets);
    } else if (query_type() == "heal" || query_type() == "utility") {
        message("action", "You attempt to " + query_name() + " towards " + names + ".", source);
        message("action", source->query_cap_name() + " attempts to " + query_name() + " towards you.", targets - ({ source }));
    }

    names = conjunction(map(targets, (: $1 == $(source) ? reflexive($(source)) : $1->query_cap_name() :)));
    if (query_type() == "attack") {
        message("action", source->query_cap_name() + " attempts to " + query_name() + " " + names + "!", environment(source), ({ source, targets... }));
    } else if (query_type() == "heal" || query_type() == "utility") {
        message("action", source->query_cap_name() + " attempts to " + query_name() + " towards " + names + ".", environment(source), ({ source }));
    }
}

/**
 * Display a message about failing to use an ability.
 *
 * @param {STD_LIVING} source the source of the fail
 * @param {STD_LIVING} target the target of the fail
 * @param limb the limb targeted, if any
 */
void ability_message_fail (object source, object target, string limb) {
    if (query_type() == "attack") {
        message("ability miss", "You miss your " + query_name() + " attempt on " + target->query_cap_name() + "!", source);
        message("ability miss", source->query_cap_name() + " misses " + possessive(source) + " " + query_name() + " attempt on you!", target);
        message("ability miss", source->query_cap_name() + " misses " + possessive(source) + " " + query_name() + " attempt on " + target->query_cap_name() + "!", environment(source), ({ source, target }));
    } else if (query_type() == "heal" || query_type() == "utility") {
        if (source == target) {
            message("action", "Your " + query_name() + " fails to affect yourself.", source);
            message("ability miss", possessive_noun(source->query_cap_name()) + " " + query_name() + " fails to affect " + reflexive(source) + ".", environment(source), ({ source }));
        } else {
            message("ability miss", "Your " + query_name() + " fails to affect " + target->query_cap_name() + ".", source);
            message("ability miss", possessive_noun(source->query_cap_name()) + " " + query_name() + " fails to affect you.", target);
            message("ability miss", possessive_noun(source->query_cap_name()) + " " + query_name() + " fails to affect " + target->query_cap_name() + ".", environment(source), ({ source, target }));
        }
    }
}

/**
 * Display a message about succeeding to use an ability.
 *
 * @param {STD_LIVING} source the source of the success
 * @param {STD_LIVING} target the target of the success
 * @param limb the limb targeted, if any
 */
void ability_message_success (object source, object target, string limb) {
    string who, you, plural = pluralize(query_name());

    if (query_type() == "attack") {
        if (limb) {
            who = possessive_noun(target->query_cap_name()) + " " + limb;
            you = "your " + limb;
        } else {
            who = target->query_cap_name();
            you = "you";
        }
        message("action", "You " + query_name() + " " + who + "!", source);
        message("action", source->query_cap_name() + " " + plural + " " + you + "!", target);
        message("action", source->query_cap_name() + " " + plural + " " + who + "!", environment(source), ({ source, target }));
    } else if (query_type() == "heal" || query_type() == "utility") {
        if (source == target) {
            message("action", "You " + query_name() + " towards yourself.", source);
            message("action", source->query_cap_name() + " " + plural + " towards " + reflexive(source) + " effectively.", environment(source), ({ source, target }));
        } else {
            who = target->query_cap_name();
            you = "you";
            message("action", "You " + query_name() + " towards " + who + " effectively.", source);
            message("action", source->query_cap_name() + " " + plural + " towards " + you + " effectively.", target);
            message("action", source->query_cap_name() + " " + plural + " towards " + who + " effectively.", environment(source), ({ source, target }));
        }
    }
}

/**
 * Display the amount of damage or heal to any characters that are immortal or
 * have a debug property flag.
 *
 * @param {STD_CHARACTER} source
 * @param {STD_CHARACTER} target
 * @param damage the amount of damage being referenced
 */
private void ability_debug_message (object source, object target, int damage) {
    string phrase;
    if (query_type() == "attack") {
        phrase = "%^ORANGE%^Damage:%^RESET%^";
    } else if (query_type() == "heal") {
        phrase = "%^CYAN%^Heal:%^RESET%^";
    }
    if (source && (source->query_immortal() || source->query_property("debug"))) {
        message("action", phrase + " " + damage, source);
    }
    if (target && target != source && (target->query_immortal() || target->query_property("debug"))) {
        message("action", phrase + " " + damage, target);
    }
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

    if (__NumTargets < sizeof(targets)) {
        return;
    }

    l = min(({ sizeof(targets), __NumTargets }));

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
    if (sizeof(targets) && __NumTargets == 1) {
        handle_ability_use(previous_object(), ({ targets[0] }));
        return;
    }
    if (__NumTargets > 1) {
        handle_ability_use(previous_object(), targets[0..__NumTargets-1]);
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
    __NumTargets = 1;
    if (query_name() != "ability") {
        add_rules(({ "", "LIV", "LVS", }));
    }
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax(query_name()+" ([target])");
}