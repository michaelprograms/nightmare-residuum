#include <verb.h>

inherit STD_VERB;

nosave private mapping __Reqs;
nosave private mapping __SkillPowers;
nosave private mapping __Weapons = ([ ]);

/* ----- ability requirements ----- */

void set_ability_requirements (mapping reqs) {
    /* Data format:
    "anyone|class|species": ([
        "level" : 1,
        "skills" : ([ "melee attack": 1, ]),
        "stats" : ([ "strength": 1, ]),
    ]),
    */
    __Reqs = reqs;
}
int verify_ability_requirements (object source) {
    if (!source || !source->is_living()) return 0;

    if (source->query_immortal()) return 1;
    // if (source->is_monster() || source->is_npc()) return 1;

    // no requirements exist
    if (!sizeof(__Reqs)) return 1;

    foreach (string key,mapping value in __Reqs) {
        if (source->query_class() != key && source->query_species() != key && key != "anyone") continue;
        if (intp(value["level"])) {
            if (source->query_level() < value["level"]) return 0;
        }
        if (mapp(value["skills"])) {
            foreach (string skill,int num in value["skills"]) {
                if (source->query_skill(skill) < num) return 0;
            }
        }
        if (mapp(value["stats"])) {
            foreach (string stat,int num in value["stats"]) {
                if (source->query_stats(stat) < num) return 0;
            }
        }
        return 1;
    }
    return 0;
}

/* ----- ability weapons ----- */

void set_weapons (mapping weapons) {
    /*
    ([
        "knife": ({ 1 }),
        "sword": ({ 1, 2 }),
        "blunt": ({ 2, }),
    ])
    */
    __Weapons = weapons;
}
object query_best_weapon (object source) {
    object weapon;
    string *types, t;
    int wc;

    types = keys(__Weapons);

    foreach (object w in source->query_wielded_weapons()) {
        if (sizeof(types)) {
            t = w->query_type();
            if (member_array(t, types) == -1) continue;
            if (member_array(w->query_hands(), __Weapons[t]) == -1) continue;
        }
        if (w->query_wc() <= wc) continue;
        wc = w->query_wc();
        weapon = w;
    }

    return weapon;
}

/* ----- skill powers ----- */

mapping query_skill_powers () {
    return __SkillPowers;
}
void set_skill_powers (mapping powers) {
    if (undefinedp(powers) || !mapp(powers)) error("Bad argument 1 to ability->set_skill_powers");

    __SkillPowers = powers;
}
int query_total_skill_power () {
    int total = 0;
    foreach (string key,int value in __SkillPowers) {
        total += value;
    }
    return total;
}

/* ----- cost ----- */

mapping query_cost () {
    string vitalType;
    mapping cost = ([
        "sp": 0,
        "mp": 0,
    ]);

    foreach (string key,int value in __SkillPowers) {
        switch (key) {
        case "psionic":
            vitalType = "mp";
            break;
        case "ranged": case "melee": default:
            vitalType = "sp";
            break;
        }
        cost[vitalType] += value/2 + (random(value/2) + 1);
    }

    return cost;
}

/* ----- damage ----- */

int calculate_damage (object source, object target, string limb) {
    int damage;
    int sourceStat, targetStat;
    int sourceSkill, targetSkill;

    // base damage
    damage = random(5 + source->query_level());
    damage += random(source->query_stat("luck") * 5 / 100 + 1);

    foreach (string key,int value in __SkillPowers) {
        switch (key) {
            case "psionic":
                sourceStat = source->query_stat("intelligence");
                damage += random(source->query_mp() * 5 / 100 + 1);
                break;
            case "ranged":
                sourceStat = source->query_stat("agility");
                damage += random(source->query_sp() * 5 / 100 + 1);
                break;
            case "melee": default:
                sourceStat = source->query_stat("strength");
                damage += random(source->query_sp() * 5 / 100 + 1);
                break;
        }
        damage += (sourceStat * 50 / 100) + random(sourceStat * 50 / 100 + 1);

        sourceSkill = source->query_skill(key + " attack");
        targetSkill = target->query_skill(key + " defense");
        damage += (sourceSkill * 20 / 100) + random(sourceSkill * 80 / 100 + 1);
        damage -= (targetSkill * 20 / 100) + random(targetSkill * 80 / 100 + 1);
    }

    // apply target mitigations
    targetStat = target->query_stat("endurance");
    damage -= ((targetStat * 10 / 100) + random(targetStat * 10 / 100 + 1));
    damage -= random(10 + target->query_level());
    damage -= random(target->query_stat("luck") * 10 / 100 + 1);
    damage -= target->query_limb_armor(limb);

    if (damage < 0) {
        damage = 0;
    }

    return damage;
}

/* ----- difficulty factor ----- */


nosave private int __DifficultyFactor;

void set_difficulty_factor (int factor) {
    if (undefinedp(factor) || !intp(factor)) error("Bad argument 1 to ability->set_difficulty_factor");
    __DifficultyFactor = factor;
}
int query_difficulty_factor () {
    if (!__DifficultyFactor) {
        __DifficultyFactor = 100;
    }
    return __DifficultyFactor;
}

/* ----- ability success ----- */

int is_ability_successful (object source, object target) {
    int sourceN = 0, targetN = 0;
    int chance = 100;
    int powerTotal = query_total_skill_power();

    // @TODO if (target->query_paralyzed()) return 100;

    foreach (string key,int value in __SkillPowers) {
        if (key == "psionic") {
            sourceN += source->query_stat("intelligence") * value / powerTotal;
            targetN += target->query_stat("perception") * value / powerTotal;
        } else if (key == "ranged") {
            sourceN += source->query_stat("agility") * value / powerTotal;
            targetN += target->query_stat("luck") * value / powerTotal;
        } else { // melee and all else
            sourceN += source->query_stat("strength") * value / powerTotal;
            targetN += target->query_stat("endurance") * value / powerTotal;
        }
    }

    targetN = targetN * query_difficulty_factor() / 100;

    // success range is 10% to 100%
    if (sourceN < targetN) {
        chance = sourceN * 100 / targetN;
        chance = max(({ 10, min(({ 100, chance })) }));
    }
    return (1+random(100)) <= chance;
}

void ability_message_attempt (object source, object target, string limb) {
    message("action", "You attempt to " + query_name() + " " + target->query_cap_name() + "!", source);
    message("action", source->query_cap_name() + " attempts to " + query_name() + " you!", target);
    message("action", source->query_cap_name() + " attempts to " + query_name() + " " + target->query_cap_name() + "!", environment(source), ({ source, target }));
}

void ability_message_fail (object source, object target, string limb) {
    message("action", "You miss your " + query_name() + " attempt on " + target->query_cap_name() + "!", source);
    message("action", source->query_cap_name() + " misses " + possessive(source) + " " + query_name() + " attempt on you!", target);
    message("action", source->query_cap_name() + " misses " + possessive(source) + " " + query_name() + " attempt on " + target->query_cap_name() + "!", environment(source), ({ source, target }));
}

void ability_message_success (object source, object target, string limb) {
    string myMsg, yourMsg, envMsg;

    if (limb) {
        myMsg = "You " + query_name() + " " + possessive_noun(target->query_cap_name()) + " " + limb + "!\n";
        yourMsg = source->query_cap_name() + " " + pluralize(query_name()) + " your " + limb + "!\n";
        envMsg = source->query_cap_name() + " " + pluralize(query_name()) + " " + possessive_noun(target->query_cap_name()) + " " + limb + "!\n";
    } else {
        myMsg = "You " + query_name() + " " + target->query_cap_name() + "!\n";
        yourMsg = source->query_cap_name() + " " + pluralize(query_name()) + " you!\n";
        envMsg = source->query_cap_name() + " " + pluralize(query_name()) + " " + target->query_cap_name() + "!\n";
    }

    message("action", myMsg, source);
    message("action", yourMsg, target);
    message("action", envMsg, environment(source), ({ source, target }));
}

private void handle_ability_use (object source, object target) {
    mapping cost;
    int damage;
    object weapon;
    string limb;

    if (!verify_ability_requirements(source)) {
        message("action", "You cannot do that.", source);
        return 0;
    }

    if (!target && !(target = source->query_target_hostile())) {
        message("action", "You have no hostile targets present.", source);
        return;
    }

    if (__Weapons["melee"]) {
        if (sizeof(source->query_wieldable_limbs()) < __Weapons["melee"][0]) {
            message("action", "You do not have any free hands.", source);
            return;
        }
    } else if (sizeof(__Weapons) && !(weapon = query_best_weapon(source))) {
        message("action", "You are not wielding the correct type of weapon.", source);
        return;
    }

    // determine cost
    cost = query_cost();

    // verify vitals can pay cost
    if (cost["sp"] > 0) {
        if (source->query_sp() < cost["sp"]) {
            message("action", "You are too tired to " + query_name() + ".", source);
            return;
        }
    }
    if (cost["mp"]) {
        if (source->query_mp() < cost["mp"]) {
            message("action", "You are too drained to " + query_name() + ".", source);
            return;
        }
    }

    // check source vitals
    if (cost["sp"] > 0) {
        if (source->query_sp() < cost["sp"]) {
            message("action", "You are too drained to " + query_name() + ".", source);
            return;
        }
    }
    if (cost["mp"]) {
        if (source->query_mp() < cost["mp"]) {
            message("action", "You are too tired to " + query_name() + ".", source);
            return;
        }
    }
    // update source vitals
    if (cost["sp"] > 0) {
        source->add_sp(-cost["sp"]);
    }
    if (cost["mp"]) {
        source->add_mp(-cost["mp"]);
    }
    // update statuses
    source->set_busy(2);
    source->add_hostile(target);
    target->add_hostile(source);
    // @TODO re-enable this when determing busy vs disable
    // source->set_disable(2);

    limb = target->query_random_limb();

    // send attempt and success or fail messages
    this_object()->ability_message_attempt(source, target, limb);
    if (is_ability_successful(source, target)) {
        this_object()->ability_message_success(source, target, limb);
    } else {
        this_object()->ability_message_fail(source, target, limb);
        return;
    }

    // determine damage
    damage = calculate_damage(source, target, limb);
    target->handle_damage(damage, limb, source);
    if (source->query_immortal() || source->query_property("debug")) {
        message("action", "%^ORANGE%^Damage:%^RESET%^ " + damage, source);
    }

    if (target && (target->query_immortal() || target->query_property("debug"))) {
        message("action", "%^ORANGE%^Damage:%^RESET%^ " + damage, target);
    }

    // @TODO send damage messages

    // train relevant skills
    foreach (string key,int value in __SkillPowers) {
        source->train_skill(key + " attack", 1.0 + value / 100.0);
        if (target) {
            target->train_skill(key + " defense", 1.0 + value / 100.0);
        }
    }
}

/* ----- help ----- */

string help (object char) {
    string result, *tmp;
    int n;

    result = ::help(char);

    if (sizeof(__Reqs)) {
        foreach (string key,mapping value in __Reqs) {
            result += "\n%^CYAN%^BOLD%^" + capitalize(key) + " Requirements%^RESET%^\n";

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
    if (n = sizeof(__Weapons)) {
        result += "\n%^CYAN%^BOLD%^Weapons%^RESET%^\n";
        foreach (string key,int *value in __Weapons) {
            result += implode(map(value, (: cardinal($1)+" handed "+$(key) :)), ", ") + "\n";
        }
    }

    return result;
}

/* ----- applies ----- */

int direct_verb_liv (mixed args...) {
    return 1;
}

mixed can_verb_liv (mixed args...) {
    return can_verb_rule(args);
}
mixed can_verb (mixed args...) {
    return can_verb_rule(args);
}

// Handle input
void do_verb_liv (mixed args...) {
    object target;

    // verify target
    if (arrayp(args)) {
        target = args[1];
    }

    handle_ability_use(previous_object(), target);
}

// Handle no input
void do_verb_rule (mixed args...) {
    handle_ability_use(previous_object(), 0);
}

void create () {
    ::create();
    __Reqs = ([ ]);
    __SkillPowers = ([ ]);
    if (query_name() != "ability") {
        add_rules(({ "", "LIV", }));
    }
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax(query_name()+" ([target])");
}