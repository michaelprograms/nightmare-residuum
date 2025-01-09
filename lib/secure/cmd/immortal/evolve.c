inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("evolve (-c=class) (-l=[1+]) (-s=[1-100]) ([target])");
    set_help_text("The evolve command is used to adjust a target's class, level, or stats.");
}

/**
 *
 * @param {STD_CHARACTER} tc
 * @param {STD_CHARACTER} target
 * @param type
 * @param old
 * @param now
 */
void display (object tc, object target, string type, string old, string now) {
    if (tc == target) {
        message("action", "You have changed your " + type + " from " + old + " to " + now + ".", tc);
    } else {
        message("action", "You have changed " + possessive_noun(target) + " " + type + " from " + old + " to " + now + ".", tc);
        message("action", tc->query_cap_name() + " has changed your " + type + " from " + old + " to " + now + ".", target);
    }
}

void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    string tmp;
    int statMax, statTarget;

    if (!input && !sizeof(flags)) {
        message("action", "Syntax: evolve (-c=class) (-l=[1+]) (-s=[1-100]) ([target])", tc);
        return;
    }
    if (input) {
        target = determine_immortal_target(tc, input);
    }

    if (sizeof(flags["c"]) && target->query_class() != flags["c"]) {
        tmp = target->query_class();
        target->set_class(flags["c"]);
        display(tc, target, "class", tmp, target->query_class());
    }

    if (sizeof(flags["l"]) && target->query_level() != to_int(flags["l"])) {
        tmp = ""+target->query_level();
        target->set_level(to_int(flags["l"]));
        display(tc, target, "level", tmp, ""+target->query_level());
    }

    if (sizeof(flags["s"]) && to_int(flags["s"]) > 0 && to_int(flags["s"]) <= 100) {
        statTarget = to_int(flags["s"]);
    } else {
        statTarget = 80;
    }
    foreach (string stat in ({ "strength", "perception", "endurance", "charisma", "intelligence", "agility", "luck", })) {
        statMax = D_CLASS->query_max_stat(target->query_class(), stat, target->query_level());
        target->set_stat(stat, statMax * statTarget / 100);
    }


    message("action", "Evolution complete.", tc);
}