inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "STR with OBJ", }));
    set_syntax("lock [door|direction] with [key]");
    set_help_text("Your character will attempt to lock any doors matching the provided input.");
    set_help_similar(({ "unlock", "open", "close", "go", }));
}

/* ----- rule: "" ----- */

mixed can_lock () {
    return "Lock what?";
}

/* ----- rule: "STR with OBJ" ----- */

mixed can_lock_str_with_obj (mixed args...) {
    return 1;
}

mixed do_lock_str_with_obj (mixed args...) {
    object po = previous_object(), env = environment(po), key;
    string str;

    if (sizeof(args) > 1) {
        str = args[0];
        key = args[1];
    }

    if (env->handle_lock(po, str, key->query_name())) {
        return 1;
    }

    message("action", "There is no " + str + " to lock here.", po);
    return 1;
}