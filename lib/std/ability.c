#include <verb.h>

inherit STD_VERB;

private string __AbilityName;
private mapping __SkillPowers;

void create () {
    ::create();
    __AbilityName = split_path(base_name())[1];
    __SkillPowers = ([ ]);
    if (__AbilityName != "ability") {
        add_rules(({ "", "LIV", }));
    }
}

string query_name () {
    return __AbilityName;
}

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
        cost[vitalType] += value * 2 + (random(value) + 1);
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

    // determine cost
    cost = query_cost();


    // verify vitals can pay cost
    if (cost["sp"] > 0) {
        if (source->query_sp() < cost["sp"]) {
            message("action", "You are too drained to " + __AbilityName + ".\n", source);
            return;
        }
    }
    if (cost["mp"]) {
        if (source->query_mp() < cost["mp"]) {
            message("action", "You are too tired to " + __AbilityName + ".\n", source);
            return;
        }
    }

    // send messages
    this_object()->handle_hit_msg(source, target);

    // check source vitals
    if (cost["sp"] > 0) {
        if (source->query_sp() < cost["sp"]) {
            message("action", "You are too drained to " + __AbilityName + ".\n", source);
            return;
        }
    }
    if (cost["mp"]) {
        if (source->query_mp() < cost["mp"]) {
            message("action", "You are too tired to " + __AbilityName + ".\n", source);
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

void do_verb_liv (mixed args...) {
    object source = previous_object(), target;

    // verify target
    if (arrayp(args)) {
        target = args[1];
    } else {
        message("action", "You have no hostile targets present.\n", source);
        return;
    }

    handle_ability_use(source, target);
}

// Handle no input
void do_verb_rule (mixed args...) {
    object source = previous_object();
    object target;
    if (target = previous_object()->query_target_hostile()) {
        do_verb_liv(args[0], target, args[2], target->query_name());
    } else {
        message("action", "You have no hostile targets present.\n", source);
    }
}