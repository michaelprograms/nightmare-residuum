void command (string input, mapping flags) {
    object ob;
    string name;

    if (!input) {
        message("action", "Syntax: dest [ob]\n", this_character());
        return;
    }

    ob = present(input, this_character());
    if (!ob) ob = present(input, environment(this_character()));
    if (ob) {
        name = ob->is_living() ? ob->query_cap_name() : ob->query_name();
        message("action", "You dest " + name + ".\n", this_character());
        message("action", this_character()->query_cap_name() + " dests you.\n", ob);
        message("action", this_character()->query_cap_name() + " dests " + name + ".\n", environment(this_character()), ({ this_character(), ob }));
        if (!ob->handle_remove()) {
            message("action", "dest: cannot dest " + ob->query_name() + ".\n", this_character());
        }
    } else {
        message("action", "dest: cannot find " + input + ".\n", this_character());
    }
}
