#include <verb.h>

inherit STD_VERB;

nosave private mapping __Reqs;
nosave private mapping __SkillPowers;
nosave private mapping __Weapons = ([ ]);
nosave private string __Type;
nosave private int __NumTargets = 1;

/* ----- ability requirements ----- */

mapping query_ability_requirements () {
    return __Reqs;
}
void set_ability_requirements (mapping reqs) {
    /* Data format:
    "anyone|class|species|NPC": ([
        "level" : 1,
        "skills" : ([ "brawl attack": 1, ]),
        "stats" : ([ "strength": 1, ]),
    ]),
    */
    __Reqs = reqs;
}
int verify_ability_requirements (object source) {
    if (!source || !source->is_living()) return 0;

    if (source->query_immortal()) return 1;

    // no requirements exist
    if (!sizeof(__Reqs)) return 1;

    foreach (string key,mapping value in __Reqs) {
        if (key == "NPC") {
            if (!source->is_npc()) {
                continue;
            }
        } else if (source->query_class() != key && source->query_species() != key && key != "anyone") {
            continue;
        }
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

/* ----- ability type ----- */

void set_type (string type) {
    if (member_array(type, ({ "attack", "heal", "utility", })) == -1) {
        error("Bad argument 1 to ability->set_type");
    }
    __Type = type;
}
string query_type () {
    return __Type;
}

/* ----- ability targets ----- */

void set_targets (int n) {
    if (undefinedp(n) || !intp(n) || n < 0) {
        error("Bad argument 1 to ability->set_targets");
    }
    __NumTargets = n;
}
int query_targets () {
    return __NumTargets;
}

/* ----- ability weapons ----- */

void set_weapons (mapping weapons) {
    /* Data format:
    ([
        "blade|blunt": ({ 1, 2 }),
        "brawl": ({ 1 }),
    ])
    */
    // Validate weapons
    if (undefinedp(weapons) || !mapp(weapons)) {
        error("Bad argument 1 to ability->set_weapons");
    }
    foreach (string key,int *hands in weapons) {
        if (member_array(key, ({ "blade", "blunt", "brawl" })) == -1) {
            error("Bad argument (keys) to ability->set_weapons");
        }
        foreach (int hand in hands) {
            if (hand < 1 || hand > 2) {
                error("Bad argument (values) to ability->set_weapons");
            }
        }
    }
    __Weapons = weapons;
}
mapping query_weapons () {
    return __Weapons;
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
        case "ranged": case "brawl": default:
            vitalType = "sp";
            break;
        }
        cost[vitalType] += value/2 + (random(value/2) + 1);
    }

    return cost;
}

/* ----- calculations ----- */

int calculate_heal (object source, object target, string limb) {
    int damage;
    int sourceStat, targetStat;
    int n;

    // base heal
    damage = random(5 + source->query_level());
    damage += random(source->query_stat("luck") * 5 / 100 + 1);

    // skill powers
    foreach (string key,int value in __SkillPowers) {
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
            break;
        }
        damage += source->query_skill(key) * n / 4 + random(source->query_skill(key) * (4-n) / 4);
        sourceStat = source->query_stat("intelligence");
        damage += (sourceStat * 50 / 100) + random(sourceStat * 50 / 100 + 1);
    }

    targetStat = target->query_stat("charisma");
    damage += ((targetStat * 10 / 100) + random(targetStat * 10 / 100 + 1));

    return damage;
}

int calculate_damage (object source, object target, string limb) {
    int damage;
    int sourceStat, targetStat;
    int sourceSkill, targetSkill;
    int spRatio, mpRatio;

    // base damage
    damage = random(5 + source->query_level());
    damage += random(source->query_stat("luck") * 5 / 100 + 1);

    foreach (string key,int value in __SkillPowers) {
        switch (key) {
            case "psionic":
                sourceStat = source->query_stat("intelligence");
                mpRatio ++;
                break;
            case "ranged":
                sourceStat = source->query_stat("agility");
                spRatio ++;
                break;
            case "brawl": default:
                sourceStat = source->query_stat("strength");
                spRatio ++;
                break;
        }
        // stat damage
        damage += (sourceStat * 50 / 100) + random(sourceStat * 50 / 100 + 1);
        // vitals damage
        if (spRatio) {
            damage += random(to_int(source->query_sp() * 5.0 * spRatio / (spRatio + mpRatio)));
        }
        if (mpRatio) {
            damage += random(to_int(source->query_mp() * 5.0 * mpRatio / (spRatio + mpRatio)));
        }

        // skill damage & mitigations
        sourceSkill = source->query_skill(key + " attack");
        targetSkill = target->query_skill(key + " defense");
        damage += (sourceSkill * value / 100) + random(sourceSkill * value / 100 + 1);
        damage -= (targetSkill * value / 100) + random(targetSkill * value / 100 + 1);
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

/* ----- success ----- */

int is_ability_successful (object source, object target) {
    int sourceN = 0, targetN = 0;
    int chance = 100;
    int powerTotal = query_total_skill_power();

    // @TODO if (target->query_paralyzed()) return 100;

    if (__Type == "attack") {
        foreach (string key,int value in __SkillPowers) {
            if (key == "psionic") {
                sourceN += source->query_stat("intelligence") * value / powerTotal;
                targetN += target->query_stat("perception") * value / powerTotal;
            } else if (key == "ranged") {
                sourceN += source->query_stat("agility") * value / powerTotal;
                targetN += target->query_stat("luck") * value / powerTotal;
            } else { // blade, blunt, brawl
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
    } else if (__Type == "heal" || __Type == "utility") {
        return 1;
    }
    return 0;
}

/* ----- messages ----- */

void ability_message_attempt (object source, object *targets) {
    string names;
    int n;

    // move source to end of list
    if ((n = member_array(source, targets)) > -1) {
        targets -= ({ source });
        targets += ({ source });
    }

    names = conjunction(map(targets, (: $1 == $(source) ? "yourself" : $1->query_cap_name() :)));

    if (__Type == "attack") {
        message("action", "You attempt to " + query_name() + " " + names + "!", source);
        message("action", source->query_cap_name() + " attempts to " + query_name() + " you!", targets);
    } else if (__Type == "heal" || __Type == "utility") {
        message("action", "You attempt to " + query_name() + " towards " + names + ".", source);
        message("action", source->query_cap_name() + " attempts to " + query_name() + " towards you.", targets - ({ source }));
    }

    names = conjunction(map(targets, (: $1 == $(source) ? reflexive($(source)) : $1->query_cap_name() :)));
    if (__Type == "attack") {
        message("action", source->query_cap_name() + " attempts to " + query_name() + " " + names + "!", environment(source), ({ source, targets... }));
    } else if (__Type == "heal" || __Type == "utility") {
        message("action", source->query_cap_name() + " attempts to " + query_name() + " towards " + names + ".", environment(source), ({ source }));
    }
}

void ability_message_fail (object source, object target, string limb) {
    if (__Type == "attack") {
        message("ability miss", "You miss your " + query_name() + " attempt on " + target->query_cap_name() + "!", source);
        message("ability miss", source->query_cap_name() + " misses " + possessive(source) + " " + query_name() + " attempt on you!", target);
        message("ability miss", source->query_cap_name() + " misses " + possessive(source) + " " + query_name() + " attempt on " + target->query_cap_name() + "!", environment(source), ({ source, target }));
    } else if (__Type == "heal" || __Type == "utility") {
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

void ability_message_success (object source, object target, string limb) {
    string who, you, plural = pluralize(query_name());

    if (__Type == "attack") {
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
    } else if (__Type == "heal" || __Type == "utility") {
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

private void ability_debug_message (object source, object target, int damage) {
    string phrase;
    if (__Type == "attack") {
        phrase = "%^ORANGE%^Damage:%^RESET%^";
    } else if (__Type == "heal") {
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

private void handle_ability_use (object source, object *targets) {
    mapping cost;
    int damage;
    object weapon;
    string limb;
    int l;

    if (!__Type) {
        error("Ability "+query_name()+" does not have an ability type set");
    }

    if (!verify_ability_requirements(source)) {
        message("action", "You cannot do that.", source);
        return 0;
    }

    if (__Type == "attack") {
        if ((!targets || !targets[0])) {
            targets = ({ source->query_target_hostile() });
        }
        if ((!targets || !targets[0])) {
            message("action", "You have no hostile targets present.", source);
            return;
        }

    } else if (__Type == "heal" || __Type == "utility") {
        if ((!targets || !targets[0])) {
            targets = ({ source });
        }
        if ((!targets || !targets[0])) {
            message("action", "You have no friendly targets present.", source);
            return;
        }
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
    // check source vitals
    if (cost["sp"] > 0 && source->query_sp() < cost["sp"]) {
        message("action", "You are too drained to " + query_name() + ".", source);
        return;
    }
    if (cost["mp"] > 0 && source->query_mp() < cost["mp"]) {
        message("action", "You are too tired to " + query_name() + ".", source);
        return;
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
    // @TODO re-enable this when determing busy vs disable
    // source->set_disable(2);

    // send attempt messages
    this_object()->ability_message_attempt(source, targets);

    foreach (object target in targets) {
        if (__Type == "attack") {
            source->add_hostile(target);
            target->add_hostile(source);
        }
        limb = target->query_random_limb();
        // send success or fail messages
        if (is_ability_successful(source, target)) {
            this_object()->ability_message_success(source, target, limb);

            if (__Type == "attack") {
                // determine damage
                damage = calculate_damage(source, target, limb);
                display_combat_message(source, target, limb, query_name(), (weapon ? weapon->query_type() : "blunt"), damage, 1);
                target->handle_damage(damage, limb, source);

                // train relevant skills
                foreach (string key,int value in __SkillPowers) {
                    source->train_skill(key + " attack", 1.0 + value / 100.0);
                    if (target) target->train_skill(key + " defense", 1.0 + value / 100.0);
                }

                ability_debug_message(source, target, damage);
            } else if (__Type == "heal") {
                // determine heal
                damage = calculate_heal(source, target, limb);
                display_heal_message(source, target, limb, damage);
                // @TODO determine hp/sp/mp
                target->add_hp(damage);

                // train relevant skills
                foreach (string key,int value in __SkillPowers) {
                    source->train_skill(key, 1.0 + value / 100.0);
                }
                ability_debug_message(source, target, damage);
            } else if (__Type == "utility") {
                this_object()->handle_utility(source, target, limb);

                foreach (string key,int value in __SkillPowers) {
                    source->train_skill(key, 1.0 + value / 100.0);
                }
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

    result += "\n%^CYAN%^BOLD%^Type%^RESET%^\n" + capitalize(__Type) + "\n";

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
    if (sizeof(args) < 2) {
        return 0;
    }

    if (!objectp(args[1]) || !args[1]->is_living()) {
        return 0;
    }
    if (__Type == "attack") {
        if (args[1] == previous_object()) {
            return 0;
        }
        return 1;
    } else if (__Type == "heal" || __Type == "utility") {
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

    if (sizeof(args) < 2) return;

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
    __SkillPowers = ([ ]);
    __NumTargets = 1;
    if (query_name() != "ability") {
        add_rules(({ "", "LIV", "LVS", }));
    }
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax(query_name()+" ([target])");
}