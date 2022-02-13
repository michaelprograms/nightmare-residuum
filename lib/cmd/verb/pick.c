inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ" }));
}

int can_pick () { return 1; }
void do_pick () {
    write("Pick what?\n");
}

int can_pick_obj (object ob, string verb) { return 1; }
void do_pick_obj (object ob, string verb) {
    object p;

    if (ob->query_picks() < 1 || !ob->query_pickable()) {
        message("verb", capitalize(ob->query_name()) + " has nothing to pick.\n", this_character());
        return;
    }

    ob->handle_pick();
    p = clone_object(ob->query_pickable());
    p->handle_move(this_character());
    message("verb", "You pick " + p->query_name() + " from " + ob->query_name() + ".\n", ({this_character()}));
    message("verb", this_character()->query_name() + " picks " + p->query_name() + " from " + ob->query_name() + ".\n", environment(this_character())->query_living_contents() - ({this_character()}));
}