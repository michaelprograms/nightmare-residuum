#define BLUNT_VERBS ({ "tap", "tickle", "sting", "hurt", "strike", "strike", "send", "knock", "slam", "blast", "destroy", "destroy", })
#define BLUNT_ADVERBS ({ "innocently", "lightly", 0, "mildly", "badly", 0, "a bone crushing blow", 0, "mercilessly", "powerfully", 0, "utterly", })

#define SHARP_VERBS ({ "tap", "tickle", "sting", "graze", "cut", "slice", "slice", "shear", "strike", "mutilate", "dismember", "destroy", })
#define SHARP_ADVERBS ({ 0, 0, "sharply", 0, 0, 0, "horribly", "to pieces", "letting blood", 0, 0, "utterly", })

string possessive (mixed value);

void display_combat_message (object source, object target, string limb, mixed weapon, string type, int damage) {
    string sourceMsg, targetMsg, envMsg;
    string weaponName;
    int percent;
    string verb, verbs, adverb, sourcePossessive;

    if (!source || !target || !weapon || !type) return;

    weaponName = objectp(weapon) ? weapon->query_name() : weapon;
    percent = to_int(damage * 100.0 / target->query_max_hp());

    if (percent < 1) {
        verb = "hit";
        adverb = "ineffectively";
    } else {
        int i = 0;
        switch(percent) {
            case 27..100: i = 11; break;
            case 23..26: i = 10; break;
            case 19..22: i = 9; break;
            case 16..18: i = 8; break;
            case 13..15: i = 7; break;
            case 10..12: i = 6; break;
            case 8..9: i = 5; break;
            case 6..7: i = 4; break;
            case 4..5: i = 3; break;
            case 3: i = 2; break;
            case 2: i = 1; break;
            case 1: default: i = 0; break;
        }
        if (member_array(type, ({ "blunt", "melee", "psionic", })) > -1) {
            verb = BLUNT_VERBS[i];
            adverb = BLUNT_ADVERBS[i];
        } else if (member_array(type, ({ "blade", "knife", "projectile", "ranged", })) > -1) {
            verb = SHARP_VERBS[i];
            adverb = SHARP_ADVERBS[i];
        }
    }

    sourcePossessive = possessive(source);
    verbs = pluralize(verb);
    adverb = stringp(adverb) ? " " + adverb + " " : " ";

    sourceMsg = sprintf("You %s %s%sin the %s with your %s.", verb, target->query_cap_name(), adverb, limb, weaponName);
    targetMsg = sprintf("%s %s you%sin the %s with %s %s.", source->query_cap_name(), verbs, adverb, limb, sourcePossessive, weaponName);
    envMsg = sprintf("%s %s %s%sin the %s with %s %s.", source->query_cap_name(), verbs, target->query_cap_name(), adverb, limb, sourcePossessive, weaponName);

    message("combat hit", sourceMsg, source);
    message("combat hit", targetMsg, target);
    message("combat hit", envMsg, environment(source), ({ source, target }));
}