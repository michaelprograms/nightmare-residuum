#include <verb.h>

inherit STD_VERB;

private string __Name;
private mapping __Reqs;
private mapping __SkillPowers;

void create () {
    ::create();
    __Name = split_path(base_name())[1];
    __Reqs = ([ ]);
    __SkillPowers = ([ ]);
    if (__Name != "ability") {
        add_rules(({ "", "LIV", }));
    }
}

/* ----- ability name ----- */

string query_name () {
    return __Name;
}

/* ----- ability requirements ----- */

void set_ability_requirements (mapping reqs) {
    /* Data format:
    "class|species": ([
        "level" : 5,
        "skills" : ([ "melee attack": 25, ]),
        "stats" : ([ "strength": 10, ]),
    ]),
    */
    __Reqs = copy(reqs);
}
int verify_ability_requirements (object source) {
    if (!source || !source->is_living()) return 0;

    if (source->query_immortal()) return 1;
    // if (source->is_monster() || source->is_npc()) return 1;

    // no requirements exist
    if (!sizeof(__Reqs)) return 1;

    foreach (string key,mapping value in __Reqs) {
        if (source->query_class() != key && source->query_species() != key) continue;
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

/* ----- skill powers ----- */

mapping query_skill_powers () {
    return __SkillPowers;
}
void set_skill_powers (mapping powers) {
    if (undefinedp(powers) || !mapp(powers)) error("Bad argument 1 to ability->set_skill_powers");

    __SkillPowers = powers;
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

int calculate_damage (object source, object target) {
    int damage;
    string sourceStat;

    // base damage
    damage = random(10);
    damage += (source->query_stat(sourceStat) * 10 / 100);
    damage += random(source->query_stat(sourceStat) * 10 / 100 + 1);
    damage += random(source->query_sp() * 10 / 100 + 1);
    damage += random(source->query_stat("luck") * 5 / 100 + 1);

    foreach (string key,int value in __SkillPowers) {
        switch (key) {
        case "psionic":
            sourceStat = "intelligence";
            break;
        case "ranged":
            sourceStat = "agility";
            break;
        case "melee": default:
            sourceStat = "strength";
            break;
        }

        damage += random(source->query_skill(key + " attack") * 20 / 100 + 1);
        damage -= random(target->query_skill(key + " defense") * 20 / 100 + 1);
    }

    // apply target mitigations
    damage -= (target->query_stat("endurance") * 10 / 100);
    damage -= random(target->query_stat("endurance") * 10 / 100 + 1);
    damage -= random(target->query_hp() * 10 / 100 + 1);
    damage -= random(target->query_stat("luck") * 10 / 100 + 1);
    // damage -= target->query_limb_armor(limb); // @TODO limb

    return damage;
}

/* -----  ----- */

private void handle_ability_use (object source, object target) {
    mapping cost;
    int damage;

    if (!verify_ability_requirements(source)) {
        message("action", "You cannot do that.\n", source);
        return 0;
    }

    if (!target && !(target = source->query_target_hostile())) {
        message("action", "You have no hostile targets present.\n", source);
        return;
    }

    // determine cost
    cost = query_cost();

    // verify vitals can pay cost
    if (cost["sp"] > 0) {
        if (source->query_sp() < cost["sp"]) {
            message("action", "You are too drained to " + __Name + ".\n", source);
            return;
        }
    }
    if (cost["mp"]) {
        if (source->query_mp() < cost["mp"]) {
            message("action", "You are too tired to " + __Name + ".\n", source);
            return;
        }
    }

    // send messages
    this_object()->handle_hit_msg(source, target);

    // check source vitals
    if (cost["sp"] > 0) {
        if (source->query_sp() < cost["sp"]) {
            message("action", "You are too drained to " + __Name + ".\n", source);
            return;
        }
    }
    if (cost["mp"]) {
        if (source->query_mp() < cost["mp"]) {
            message("action", "You are too tired to " + __Name + ".\n", source);
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

    // update source status
    source->set_busy(2);
    // @TODO re-enable this when determing busy vs disable
    // source->set_disable(2);

    // set hostile status
    source->add_hostile(target);
    target->add_hostile(source);

    // determine damage
    damage = calculate_damage(source, target);
    target->handle_damage(damage, source);
    if (source->query_immortal()) message("action", "%^ORANGE%^Damage:%^RESET%^ " + damage + "\n", source);

    // @TODO send damage messages

    // train relevant skills
    foreach (string key,int value in __SkillPowers) {
        source->train_skill(key + " attack", 1.0 + value / 100.0);
        if (target) {
            target->train_skill(key + " defense", 1.0 + value / 100.0);
        }
    }
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