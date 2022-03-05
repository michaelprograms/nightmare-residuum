void command (string input) {
    object ob;
    string name;

    if (!input) {
        write("Syntax: dest [ob]\n");
        return;
    }

    ob = present(input, this_character());
    if (!ob) ob = present(input, environment(this_character()));
    if (ob) {
        name = ob->query_name();
        if (ob->handle_remove()) {
            message("action", "You dest " + name + ".\n", this_character());
            message("action", this_character()->query_name() + " dests " + name + ".\n", environment(this_character()), this_character());
        } else {
            write("dest: cannot dest " + ob->query_name() + ".\n");
        }
    } else {
        write("dest: cannot find " + input + ".\n");
    }
}
