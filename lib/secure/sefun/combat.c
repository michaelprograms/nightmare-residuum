#define BLUNT_VERBS ({ "tap", "tickle", "sting", "hurt", "strike", "strike", "send", "knock", "slam", "blast", "destroy", "destroy", })
#define BLUNT_ADVERBS ({ "innocently", "lightly", 0, "mildly", "badly", 0, "a bone crushing blow", 0, "mercilessly", "powerfully", 0, "utterly", })

#define SHARP_VERBS ({ "tap", "tickle", "sting", "graze", "cut", "slice", "slice", "shear", "strike", "mutilate", "dismember", "destroy", })
#define SHARP_ADVERBS ({ 0, 0, "sharply", 0, 0, 0, "horribly", "to pieces", "letting blood", 0, 0, "utterly", })

int query_combat_tier_from_percent (int percent) {
    if (percent >= 27) {
        return 11;
    } else if (percent >= 23) {
        return 10;
    } else if (percent >= 19) {
        return 9;
    } else if (percent >= 16) {
        return 8;
    } else if (percent >= 13) {
        return 7;
    } else if (percent >= 10) {
        return 6;
    } else if (percent >= 8) {
        return 5;
    } else if (percent >= 6) {
        return 4;
    } else if (percent >= 4) {
        return 3;
    } else if (percent == 3) {
        return 2;
    } else if (percent == 2) {
        return 1;
    }
    return 0;
}

/* -----  ----- */

void combat_hit_message (object source, object target, string limb, mixed weapon, string type, int damage, int crit, int isAbility) {
    string sourceMsg, targetMsg, envMsg;
    string weaponName;
    int percent;
    string critically, verb, verbs, adverb, sourcePossessive;
    int i = 0;

    weaponName = objectp(weapon) ? weapon->query_name() : weapon;
    percent = to_int(damage * 100.0 / target->query_max_hp());
    critically = crit ? "critically " : "";

    if (percent < 1) {
        verb = "hit";
        adverb = "ineffectively";
    } else {
        i = query_combat_tier_from_percent(percent);
        if (member_array(type, ({ "blunt", "brawl", "psionic", })) > -1) {
            verb = BLUNT_VERBS[i];
            adverb = BLUNT_ADVERBS[i];
        } else if (member_array(type, ({ "blade", "ranged", })) > -1) {
            verb = SHARP_VERBS[i];
            adverb = SHARP_ADVERBS[i];
        }
    }

    sourcePossessive = SEFUN->possessive(source);
    verbs = pluralize(verb);
    adverb = adverb ? " " + adverb + " " : " ";

    sourceMsg = sprintf("You %s%s %s%sin the %s with your %s.", critically, verb, target->query_cap_name(), adverb, limb, weaponName);
    targetMsg = sprintf("%s %s%s you%sin the %s with %s %s.", source->query_cap_name(), crit ? "critically " : "", verbs, adverb, limb, sourcePossessive, weaponName);
    envMsg = sprintf("%s %s%s %s%sin the %s with %s %s.", source->query_cap_name(), crit ? "critically " : "", verbs, target->query_cap_name(), adverb, limb, sourcePossessive, weaponName);

    if (isAbility) {
        message("ability hit", sourceMsg, source);
        message("ability hit", targetMsg, target);
        message("ability hit", envMsg, environment(source), ({ source, target }));
    } else {
        message("combat hit", sourceMsg, source);
        message("combat hit", targetMsg, target);
        message("combat hit", envMsg, environment(source), ({ source, target }));
    }
}

void combat_heal_message (object source, object target, string limb, int damage) {
    if (target->query_hp() < target->query_max_hp()) {
        message("combat heal", SEFUN->possessive_noun(target->query_cap_name()) + " wounds heal slightly.", environment(target), target);
        message("combat heal", "Your wounds heal slightly.", target);
    }
}

void combat_miss_message (object source, object target, mixed weapon) {
    string type, name, possessive = SEFUN->possessive(source);

    if (objectp(weapon)) {
        type = weapon->query_type();
        name = weapon->query_name();
    } else {
        type = "brawl";
        name = weapon;
    }
    message("combat miss", "You miss " + target->query_cap_name() + " with your " + name + ".", source);
    message("combat miss", source->query_cap_name() + " misses you with " + possessive + " " + name + ".", target);
    message("combat miss", source->query_cap_name() + " misses " + target->query_cap_name() + " with " + possessive + " " + name + ".", environment(source), ({ source, target }));
}

void combat_block_message (object source, object target) {
    object shield = source->query_worn_shield();
    string possessive = SEFUN->possessive(target->query_cap_name());
    message("combat miss", "You block " + source->query_cap_name() + (shield ? " with your " + shield->query_name() : "") + ".", target);
    message("combat miss", target->query_cap_name() + " blocks you" + (shield ? " with " + possessive + " " + shield->query_name() : "") + ".", source);
    message("combat miss", target->query_cap_name() + " blocks " + source->query_cap_name() + (shield ? " with " + possessive + " " + shield->query_name() : "") + ".", environment(target), ({ source, target }));
}

/* -----  ----- */

void initiate_combat (object source, object target) {
    message("attack", "You attack " + target->query_cap_name() + "!", source);
    message("attack", source->query_cap_name() + " attacks you!", target);
    message("attack", source->query_cap_name() + " attacks " + target->query_cap_name() + "!", environment(source), ({ source, target }));

    source->add_hostile(target);
    target->add_hostile(source);
}

object *present_hostiles (object source) {
    object env = environment(source);
    return filter(source->query_hostiles(), (: environment($1) == $(env) :));
}
object present_hostile (object source) {
    object *hostiles = present_hostiles(source);
    return sizeof(hostiles) ? hostiles[0] : 0;
}

/* -----  ----- */

mapping *combat_table (object source, object target, int hits) {
    mapping *table = ({ });
    int levelAdjust = to_int(5 + (target->query_level() - source->query_level()) / 5.0);
    int miss, /*resist,*/ block, parry, evade, criticalHit;

    miss = max(({ 0, levelAdjust + hits }));
    // resist = 0.0;
    block = (target && target->query_worn_shield() ? max(({ 0, levelAdjust - hits })) : 0);
    parry = (sizeof(target && target->query_wielded_weapons()) ? max(({ 0, levelAdjust - hits })) : 0);
    evade = max(({ 0, levelAdjust - hits }));
    criticalHit = max(({ 0, levelAdjust - hits }));

    if (miss > 0) {
        table += ({ ([ "id": "miss", "value": miss ]) });
    }
    // TODO: Add resist
    // if (resist > 0) {
    //     table += ({ ([ "id": "resist", "value": resist ]) });
    // }
    if (block > 0) {
        table += ({ ([ "id": "block", "value": block ]) });
    }
    if (parry > 0) {
        table += ({ ([ "id": "parry", "value": parry ]) });
    }
    if (evade > 0) {
        table += ({ ([ "id": "evade", "value": evade ]) });
    }
    if (criticalHit > 0) {
        table += ({ ([ "id": "critical hit", "value": criticalHit ]) });
    }
    table += ({ ([ "id": "regular hit", "value": 100 ]) });
    return table;
}