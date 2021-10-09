inherit "/std/verb.c";

void create () {
    verb::create();
    add_rules(({ "", "STR", "into STR" }));
    // clear_flag(NEED_TO_SEE);
    // clear_flag(NEED_TO_BE_ALIVE);
}

mixed can_go () {
    return "Go where?";
}

mixed can_go_str (string str) {
    if (!environment(this_character())) return "You are nowhere.";
    // if (this_character()->query_paralyzed()) return "You are unable to move."; // @TODO
    if (!environment(this_character())->query_exit(str)) return "You cannot go that way.";
    return 1;
}
mixed do_go_str (string str) {
    mapping exits = environment(previous_object())->query_exits(str);

    if (undefinedp(exits) || !mapp(exits)) {
        return 0;
    }
    if (exits[str] && exits[str]["pre"] && !(evaluate(exits[str]["pre"], str))) {
        return 1;
    }
    if (exits[str] && exits[str]["room"]) {
        previous_object()->handle_go(exits[str]["room"], "walk", str);
        if (exits[str]["post"]) {
            evaluate(exits[str]["post"], str);
        }
        previous_object()->describe_environment();
        return 1;
    }
    return 0;
}

mixed can_go_into_str (string str) { return 1; }
mixed do_go_into_str (string str) {
    tell(previous_object(), "Go into "+str+"\n");
}

    // SetVerb("go");
    // SetRules("STR", "into STR");
    // SetErrorMessage("Go in which direction?");
    // SetHelp("Syntax: go <DIRECTION>\n"
    //         "        go into <PLACE>\n\n"
    //         "Moves you towards the direction you specify, or into the place "
    //         "you specify.  The command \"go into\" is synonymous with the "
    //         "\"enter\" command.  Note that most directions are pre-aliased "
    //         "for you to the proper \"go\" commands.  For example, "
    //         "\"go south\" has been pre-aliased to \"s\".\n"
    //         "See also: climb, enter, jump, fly, swim");