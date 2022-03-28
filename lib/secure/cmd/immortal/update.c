nosave private int testStart;
void done (mapping results) {
    D_TEST->display_results(results, testStart);
}

void command (string input) {
    string tmp;
    object ob;
    object *keep = ({});

    if (!input) {
        write("Update which file?\n");
        return;
    } if (input == "here") {
        input = file_name(environment(this_character()));
        if (file_size(input) == -1 && file_size(input + ".c") > 0) input += ".c";
    } else {
        if (input[0] != '/' && input[0] != '~' && input[0] != '^') {
            input = this_user()->query_shell()->query_variable("cwd") + "/" + input;
        }
        input = sanitize_path(input);
        write("Updating "+input+"...\n");
    }

    switch (file_size(input)) {
        case -1:
            write("update: " + input + " not found.\n");
            return;
        case 0:
            write("update: " + input + " is empty.\n");
            return;
    }
    ob = find_object(input);
    if (ob) {
        if (ob->is_room()) {
            foreach (object l in ob->query_living_contents()) {
                if (l->is_character()) {
                    keep += ({ l });
                    l->handle_move("/domain/Nowhere/room/void.c");
                }
            }
        }
        write("Attempting ob->handle_remove()...\n");
        ob->handle_remove();
    }
    if (ob) {
        write("Attempting destruct(ob)...\n");
        destruct(ob);
    }

    tmp = catch (load_object(input));
    if (!tmp) {
        string test = input[0..<2] + "test.c";
        message("system", input + ": Ok\n", this_user());
        foreach (object l in keep) {
            l->handle_move(input);
        }
        keep->describe_environment();
        keep = ({});
        if (file_size(test) > 0) {
            testStart = rusage()["utime"] + rusage()["stime"];
            D_TEST->process_file(test, (: done :), 1);
        }
    } else {
        message("system", input + ": Error in update\n", this_user());
    }
}