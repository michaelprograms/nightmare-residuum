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

void display_combat_message (object source, object target, string limb, mixed weapon, string type, int damage, int crit, int isAbility) {
    string sourceMsg, targetMsg, envMsg;
    string weaponName;
    int percent;
    string verb, verbs, adverb, sourcePossessive;
    int i = 0;

    if (!source || !target || !weapon || !type) {
        return;
    }

    weaponName = objectp(weapon) ? weapon->query_name() : weapon;

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

    sourceMsg = sprintf("You %s%s %s%sin the %s with your %s.", crit ? "critically " : "", verb, target->query_cap_name(), adverb, limb, weaponName);
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

void display_heal_message (object source, object target, string limb, int damage) {
    if (target->query_hp() < target->query_max_hp()) {
        message("action", SEFUN->possessive_noun(target->query_cap_name()) + " wounds heal slightly.", environment(target), target);
        message("action", "Your wounds heal slightly.", target);
    }
}

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