void command (string input) {
    object ob;

    if (!input) {
        write("Syntax: dest [ob]\n");
        return;
    }

    ob = present(input, this_character());
    if (!ob) ob = present(input, environment(this_character()));
    if (ob) {
        if (ob->handle_remove()) {
            write("You dest " + str + ".\n");
        } else {
            write("dest: cannot dest " + ob->query_name() + ".\n");
        }
    } else {
        write("dest: cannot find " + input + ".\n");
    }
}
