inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("evolve -c=class -l=level ([target])");
    set_help_text("The evolve command is used to adjust a target's class or level.");
}

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

    if (!input && !sizeof(flags)) {
        message("action", "Syntax: evolve (-l=[level]) (-c=[class]) ([target])", tc);
        return;
    }
    if (input) {
        if (find_character(input)) target = find_character(input);
        else if (present(input, environment(tc))) target = present(input, environment(tc));
    }

    if (sizeof(flags["c"]) && target->query_class() != flags["c"]) {
        tmp = target->query_class();
        target->set_class(flags["c"]);
        display(tc, target, "class", tmp, target->query_class());
    }

    if (sizeof(flags["l"]) && target->query_level() != to_int(flags["l"])) {
        tmp = target->query_level();
        target->set_level(to_int(flags["l"]));
        display(tc, target, "level", tmp, target->query_level());
    }

    message("action", "Evolution complete.", tc);
}