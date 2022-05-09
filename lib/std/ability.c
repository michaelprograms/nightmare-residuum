#include <verb.h>

inherit STD_VERB;

private string __Ability;
private string __SkillType;
private int __BasePower;

void create () {
    ::create();
    __Ability = split_path(file_name())[1];
    if (__Ability != "ability") {
        add_rules(({ "", "LIV", }));
    }
}

void set_skill_type (string type) {
    __SkillType = type;
}
void set_base_power (int p) {
    __BasePower = p;
}

/* ----- ----- */

int calculate_damage (object source, object target) {
    int damage;
    string sourceStat;

    if (__SkillType == "psionic") {
        sourceStat = "intelligence";
    } else {
        sourceStat = "strength";
    }

    // base damage
    damage = secure_random(10);
    damage += (source->query_stat(sourceStat) * 10 / 100);
    damage += secure_random(source->query_stat(sourceStat) * 10 / 100 + 1);
    damage += secure_random(source->query_sp() * 10 / 100 + 1);
    damage += secure_random(source->query_stat("luck") * 5 / 100 + 1);
    damage += secure_random(source->query_skill(__SkillType + " attack") * 20 / 100 + 1);

    // apply target mitigations
    damage -= (target->query_stat("endurance") * 10 / 100);
    damage -= secure_random(target->query_stat("endurance") * 10 / 100 + 1);
    damage -= secure_random(target->query_hp() * 10 / 100 + 1);
    damage -= secure_random(target->query_stat("luck") * 10 / 100 + 1);
    damage -= secure_random(target->query_skill(__SkillType + " defense") * 20 / 100 + 1);
    // damage -= target->query_limb_armor(limb); // @TODO limb

    return damage;
}

/* ----- applies ----- */

int direct_verb_liv (mixed args...) {
    // write(__Ability+" direct_verb_liv: " + identify(args)+"\n");
    return 1;
}

mixed can_verb_liv (mixed args...) {
    // write(__Ability+" can_verb_liv: " + identify(args)+"\n");
    return can_verb_rule(args);
}

void do_verb_liv (mixed args...) {
    object source = previous_object();
    object target;
    int damage, cost;

    // write(__Ability+" do_verb_liv args: " + identify(args) + "\n");

    // verify target
    if (arrayp(args)) {
        target = args[1];
    } else {
        message("action", "Unexpected args from " + __Ability + ": " + identify(args) + ".\n", source);
        return;
    }

    // determine cost
    cost = __BasePower * 2 + (random(__BasePower) + 1);
    if (__SkillType == "psionic") {
        if (source->query_mp() < cost) {
            message("action", "You are too drained to " + __Ability + ".\n", source);
            return;
        }
    } else {
        if (source->query_sp() < cost) {
            message("action", "You are too tired to " + __Ability + ".\n", source);
            return;
        }
    }

    // send messages
    this_object()->handle_hit_msg(source, target);

    // update source statuses
    if (__SkillType == "psionic") {
        source->set_disable(2);
        source->add_mp(-cost);
    } else {
        source->set_busy(2);
        source->add_sp(-cost);
    }

    // set hostile status
    source->add_hostile(target);
    target->add_hostile(source);

    // determine damage
    damage = calculate_damage(source, target);
    target->handle_damage(damage, source);

    // @TODO send damage messages

    // train relevant skills
    source->train_skill(__SkillType + " attack", 1.0);
    target->train_skill(__SkillType + " defense", 1.0);
}