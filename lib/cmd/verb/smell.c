inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "STR", }));
    set_syntax("smell ([scent])");
    set_help_text("Your character will attempt to smell the provided scent.");
    set_help_similar(({ "listen", "look", }));
}

/* ----- rule: "" ----- */

mixed can_smell () {
    if (!environment(previous_object())) return "There is nothing.";
    else return 1;
}
mixed do_smell () {
    object po = previous_object();
    /** @type {STD_ROOM} env */
    object env = environment(po);

    if (env->query_smell("default")) {
        previous_object()->describe_environment_senses("smell");
    } else {
        message("action", "There is nothing to smell.", po);
    }
    return 1;
}

/* ----- rule: "STR" and "at STR" ----- */

mixed can_smell_str (string str, string verb) {
    return 1;
}
void do_smell_str (mixed args...) {
    object po = previous_object();
    /** @type {STD_ROOM} env */
    object env = environment(po);
    string str;

    if (sizeof(args)) str = args[0];

    if (!str || !env->query_smell(str)) {
        message("action", "Smell what?", po);
        return;
    }
    previous_object()->describe_environment_senses("smell", str);
}
