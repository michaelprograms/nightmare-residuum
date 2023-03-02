inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "STR", }));
    set_syntax("close [door|direction]");
    set_help_text("Your character will attempt to close any doors matching the provided input.");
    set_help_similar(({ "open", "lock", "unlock", "go", }));
}

/* ----- rule: "" ----- */

mixed can_close () {
    return "Close what?";
}

/* ----- rule: "STR" ----- */

mixed can_close_str (mixed args...) {
    return 1;
}

mixed do_close_str (mixed args...) {
    object po = previous_object(), env = environment(po);
    string str;

    if (sizeof(args) > 1) {
        str = args[0];
    }

    if (member_array(str, env->query_doors()) > -1) {
        if (env->handle_close(po, str)) return 1;
    }

    message("action", "There is no " + str + " to close here.", po);
    return 1;
}