#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", "OBJ into LIV" }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("inject [item]");
    set_help_text("Your character will attempt to inject the provided item.");
    set_help_similar(({ "inject", }));
}

/* ----- OBJ ----- */

mixed can_inject () {
    return "Inject what?";
}

void do_inject_obj (object ob, string str) {
    object po = previous_object();

    message("action", "You inject " + ob->query_name() + " of " + ob->query_type() + ".", po);
    message("action", po->query_cap_name() + " injects " + ob->query_name() + " of " + ob->query_type() + ".", environment(po), po);

    ob->handle_inject(po);
}

/* ----- OBJ into LIV ----- */

mixed can_inject_obj_into_liv (mixed args...) {
    return 1;
}

void do_inject_obj_into_liv (mixed args...) {
    object po = previous_object();
    object ob = args[0];
    object target = args[1];
    string id = args[3];

    if (environment(po) != environment(target)) {
        message("action", "There is no " + id + " here.", po);
        return;
    }

    message("action", "You inject " + ob->query_name() + " of " + ob->query_type() + " into " + target->query_cap_name() + ".", po);
    message("action", po->query_cap_name() + " injects " + ob->query_name() + " of " + ob->query_type() + " into you.", target);
    message("action", po->query_cap_name() + " injects " + ob->query_name() + " of " + ob->query_type() + " into " + target->query_cap_name() + ".", environment(po), ({ po, target }));
    ob->handle_inject(target);
}