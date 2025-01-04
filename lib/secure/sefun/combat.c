#define BLUNT_VERBS ({ "tap", "tickle", "sting", "hurt", "strike", "strike", "send", "knock", "slam", "blast", "destroy", "destroy", })
#define BLUNT_ADVERBS ({ "innocently", "lightly", 0, "mildly", "badly", 0, "a bone crushing blow", 0, "mercilessly", "powerfully", 0, "utterly", })

#define SHARP_VERBS ({ "tap", "tickle", "sting", "graze", "cut", "slice", "slice", "shear", "strike", "mutilate", "dismember", "destroy", })
#define SHARP_ADVERBS ({ 0, 0, "sharply", 0, 0, 0, "horribly", "to pieces", "letting blood", 0, 0, "utterly", })

/**
 * Turn a percent of a target's health into a text description.
 *
 * @param percent an integer value of 0-100 percent of a target's health
 */
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

/**
 * Display a combat hit message.
 *
 * @param {STD_LIVING} source the source of the attack
 * @param {STD_LIVING} target the target of the attack
 * @param limb the target's limb being hit
 * @param {STD_WEAPON|string} weapon the weapon or limb used
 * @param damage how much damage this hit will do
 * @param crit flag if this was a critical hit
 * @param isAbility flag if this was from an ability
 */
void combat_hit_message (object source, object target, string limb, mixed weapon, int damage, int crit, int isAbility) {
    string sourceMsg, targetMsg, envMsg, weaponName, type;
    string critically, verb, verbs, adverb, sourcePossessive;
    int percent, i = 0;

    if (objectp(weapon)) {
        type = weapon->query_type();
        weaponName = weapon->query_name();
    } else {
        type = "brawl";
        weaponName = weapon;
    }
    critically = crit ? "critically " : "";
    percent = to_int(damage * 100.0 / target->query_max_hp());
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

/**
 * Display a combat heal message.
 *
 * @param {STD_LIVING} source the source of the heal
 * @param {STD_LIVING} target the target of the heal
 * @param limb the limb targeted (if any)
 * @param damage the amount of damage healed
 */
void combat_heal_message (object source, object target, string limb, int damage) {
    // @TODO: use limb and damage
    if (target->query_hp() < target->query_max_hp()) {
        message("combat heal", SEFUN->possessive_noun(target->query_cap_name()) + " wounds heal slightly.", environment(target), target);
        message("combat heal", "Your wounds heal slightly.", target);
    }
}

/**
 * Display a combat miss message.
 *
 * @param {STD_LIVING} source the source of the miss
 * @param {STD_LIVING} target the target of the miss
 * @param {STD_WEAPON|string} weapon the weapon or limb used
 */
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

/**
 * Display a combat block message.
 *
 * @param {STD_LIVING} source the source of the block
 * @param {STD_LIVING} target the target of the block
 */
void combat_block_message (object source, object target) {
    object shield = source->query_worn_shield();
    string possessive = SEFUN->possessive(target->query_cap_name());
    message("combat miss", "You block " + source->query_cap_name() + (shield ? " with your " + shield->query_name() : "") + ".", target);
    message("combat miss", target->query_cap_name() + " blocks you" + (shield ? " with " + possessive + " " + shield->query_name() : "") + ".", source);
    message("combat miss", target->query_cap_name() + " blocks " + source->query_cap_name() + (shield ? " with " + possessive + " " + shield->query_name() : "") + ".", environment(target), ({ source, target }));
}

/**
 * Display a combat parry message.
 *
 * @param {STD_LIVING} source the source of the parry
 * @param {STD_LIVING} target the target of the parry
 * @param {STD_WEAPON|string} weapon the weapon or limb used
 */
void combat_parry_message (object source, object target, mixed weapon) {
    string type, name, possessive = SEFUN->possessive(target);

    if (objectp(weapon)) {
        type = weapon->query_type();
        name = weapon->query_name();
    } else {
        type = "brawl";
        name = weapon;
    }
    message("combat miss", "You parry " + source->query_cap_name() + " with your " + name + ".", target);
    message("combat miss", target->query_cap_name() + " parries you with " + possessive + " " + name + ".", source);
    message("combat miss", target->query_cap_name() + " parries " + source->query_cap_name() + " with " + possessive + " " + name + ".", environment(source), ({ source, target }));
}

/**
 * Display a combat evade message.
 *
 * @param {STD_LIVING} source the source of the evade
 * @param {STD_LIVING} target the target of the evade
 */
void combat_evade_message (object source, object target) {
    message("combat miss", "You evade " + SEFUN->possessive_noun(source->query_cap_name()) + " attack.", target);
    message("combat miss", target->query_cap_name() + " evades your attack.", source);
    message("combat miss", target->query_cap_name() + " evades " + SEFUN->possessive_noun(source->query_cap_name()) + " attack.", environment(source), ({ source, target }));
}

/**
 * Calculate how much damage a hit will do.
 *
 * @param {STD_LIVING} source the source of the damage
 * @param {STD_LIVING} target the target of the damage
 * @param limb the target's limb being hit
 * @param {STD_WEAPON|string} weapon
 * @param crit flag if this was a critical hit
 */
int combat_hit_damage (object source, object target, string limb, mixed weapon, int crit) {
    // @TODO: use weapon for damage
    int dice, damage = 0;
    // calculate source damage
    damage += SEFUN->roll_die(1, 6)[0];
    dice = max(({ 1, random(source->query_stat("strength") + 1) * 5 / 100 }));
    damage += SEFUN->roll_die(dice, 6)[0];
    dice = max(({ 1, random(source->query_stat("luck") + 1) * 5 / 100 }));
    damage += SEFUN->roll_die(dice, 6)[0];
    if (crit) {
        damage = damage * 3 / 2;
    }
    // apply target mitigations
    dice = max(({ 1, random(target->query_stat("endurance") + 1) * 10 / 100 }));
    damage -= SEFUN->roll_die(dice, 6)[0];
    dice = max(({ 1, random(target->query_stat("luck") + 1) * 10 / 100 }));
    damage -= SEFUN->roll_die(dice, 6)[0];
    damage -= target->query_limb_armor(limb);
    damage -= target->query_protection();
    damage = max(({ 0, damage }));
    return damage;
}

/* -----  ----- */

/**
 * Display messages about initiating combat and add each other to their
 * hostility lists.
 *
 * @param {STD_LIVING} source the initiator of hostility
 * @param {STD_LIVING} target the recipient of hostility
 */
void initiate_combat (object source, object target) {
    message("attack", "You attack " + target->query_cap_name() + "!", source);
    message("attack", source->query_cap_name() + " attacks you!", target);
    message("attack", source->query_cap_name() + " attacks " + target->query_cap_name() + "!", environment(source), ({ source, target }));

    source->add_hostile(target);
    target->add_hostile(source);
}

/**
 * Return a list of present hostile living objects.
 *
 * @param {STD_LIVING} source the living object to check the environment of
 * @returns {STD_LIVING*} a list of valid hostile living objects
 */
object *present_hostiles (object source) {
    object env = environment(source);
    return filter(source->query_hostiles(), (: environment($1) == $(env) :));
}
/**
 * If there are any present hostile living objects, return the first one.
 *
 * @param {STD_LIVING} source the living object to check the environment of
 * @returns {STD_LIVING} a valid hostile living object
 */
object present_hostile (object source) {
    object *hostiles = present_hostiles(source);
    return sizeof(hostiles) ? hostiles[0] : 0;
}

/* -----  ----- */

/**
 * Create a combat table to determine miss/block/parry/evade/crit/hit chances.
 * Each subsequent hit in a combat round has a higher chance to miss.
 *
 * @param {STD_LIVING} source the source of the attack
 * @param {STD_LIVING} target the target of the attack
 * @param hits the n-th hit in a row this combat round
 */
mapping *combat_table (object source, object target, int hits) {
    mapping *table = ({ });
    int levelAdjust = to_int(5 + (target->query_level() - source->query_level()) / 5.0);
    int miss, /*resist,*/ block, parry, evade, criticalHit;

    miss = max(({ 0, levelAdjust + hits }));
    // resist = 0.0;
    block = target->query_worn_shield() ? max(({ 0, levelAdjust - hits })) : 0;
    parry = sizeof(target->query_wielded_weapons()) ? max(({ 0, levelAdjust - hits })) : 0;
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