// @this_object /std/ability.c

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

    if (this_object()->query_type() == "attack") {
        message("action", "You attempt to " + this_object()->query_name() + " " + names + "!", source);
        message("action", source->query_cap_name() + " attempts to " + this_object()->query_name() + " you!", targets);
    } else if (this_object()->query_type() == "heal" || this_object()->query_type() == "utility") {
        message("action", "You attempt to " + this_object()->query_name() + " towards " + names + ".", source);
        message("action", source->query_cap_name() + " attempts to " + this_object()->query_name() + " towards you.", targets - ({ source }));
    }

    names = conjunction(map(targets, (: $1 == $(source) ? reflexive($(source)) : $1->query_cap_name() :)));
    if (this_object()->query_type() == "attack") {
        message("action", source->query_cap_name() + " attempts to " + this_object()->query_name() + " " + names + "!", environment(source), ({ source, targets... }));
    } else if (this_object()->query_type() == "heal" || this_object()->query_type() == "utility") {
        message("action", source->query_cap_name() + " attempts to " + this_object()->query_name() + " towards " + names + ".", environment(source), ({ source }));
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
    if (this_object()->query_type() == "attack") {
        message("ability miss", "You miss your " + this_object()->query_name() + " attempt on " + target->query_cap_name() + "!", source);
        message("ability miss", source->query_cap_name() + " misses " + possessive(source) + " " + this_object()->query_name() + " attempt on you!", target);
        message("ability miss", source->query_cap_name() + " misses " + possessive(source) + " " + this_object()->query_name() + " attempt on " + target->query_cap_name() + "!", environment(source), ({ source, target }));
    } else if (this_object()->query_type() == "heal" || this_object()->query_type() == "utility") {
        if (source == target) {
            message("action", "Your " + this_object()->query_name() + " fails to affect yourself.", source);
            message("ability miss", possessive_noun(source->query_cap_name()) + " " + this_object()->query_name() + " fails to affect " + reflexive(source) + ".", environment(source), ({ source }));
        } else {
            message("ability miss", "Your " + this_object()->query_name() + " fails to affect " + target->query_cap_name() + ".", source);
            message("ability miss", possessive_noun(source->query_cap_name()) + " " + this_object()->query_name() + " fails to affect you.", target);
            message("ability miss", possessive_noun(source->query_cap_name()) + " " + this_object()->query_name() + " fails to affect " + target->query_cap_name() + ".", environment(source), ({ source, target }));
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
    string who, you, plural = pluralize(this_object()->query_name());

    if (this_object()->query_type() == "attack") {
        if (limb) {
            who = possessive_noun(target->query_cap_name()) + " " + limb;
            you = "your " + limb;
        } else {
            who = target->query_cap_name();
            you = "you";
        }
        message("action", "You " + this_object()->query_name() + " " + who + "!", source);
        message("action", source->query_cap_name() + " " + plural + " " + you + "!", target);
        message("action", source->query_cap_name() + " " + plural + " " + who + "!", environment(source), ({ source, target }));
    } else if (this_object()->query_type() == "heal" || this_object()->query_type() == "utility") {
        if (source == target) {
            message("action", "You " + this_object()->query_name() + " towards yourself.", source);
            message("action", source->query_cap_name() + " " + plural + " towards " + reflexive(source) + " effectively.", environment(source), ({ source, target }));
        } else {
            who = target->query_cap_name();
            you = "you";
            message("action", "You " + this_object()->query_name() + " towards " + who + " effectively.", source);
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
void ability_debug_message (object source, object target, int damage) {
    string phrase;
    if (this_object()->query_type() == "attack") {
        phrase = "%^ORANGE%^Damage:%^RESET%^";
    } else if (this_object()->query_type() == "heal") {
        phrase = "%^CYAN%^Heal:%^RESET%^";
    }
    if (source && (source->query_immortal() || source->query_property("debug"))) {
        message("action", phrase + " " + damage, source);
    }
    if (target && target != source && (target->query_immortal() || target->query_property("debug"))) {
        message("action", phrase + " " + damage, target);
    }
}