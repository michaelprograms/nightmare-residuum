inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ" }));
    set_syntax("pick ([item])");
    set_help_text("Your character will attempt to pick the provided item.");
}

int can_pick () { return 1; }
void do_pick () {
    write("Pick what?\n");
}

int can_pick_obj (object ob, string verb) { return 1; }
void do_pick_obj (object ob, string verb) {
    object p;

    if (ob->query_picks() < 1 || !ob->query_pick_item()) {
        message("action", ob->query_cap_name() + " has nothing to pick.", this_character());
        return;
    }

    ob->handle_pick();
    p = new(ob->query_pick_item());
    p->handle_move(this_character());
    message("action", "You pick " + p->query_name() + " from " + ob->query_name() + ".", this_character());
    message("action", this_character()->query_cap_name() + " picks " + p->query_name() + " from " + ob->query_name() + ".", environment(this_character()), this_character());
}