nosave private int testStart;
void done (mapping results) {
    D_TEST->display_results(results, testStart);
}

void command (string input, mapping flags) {
    string tmp;
    object ob;
    object *keep = ({ });

    if (!input) {
        message("system", "Update which file?\n", this_user());
        return;
    } if (input == "here") {
        input = program_name(environment(this_character()));
        if (file_size(input) == -1 && file_size(input + ".c") > 0) input += ".c";
    } else {
        if (input[0] != '/' && input[0] != '~' && input[0] != '^') {
            input = this_user()->query_shell()->query_variable("cwd") + "/" + input;
        }
        input = sanitize_path(input);
        message("system", "Updating " + input + (flags["R"] ? " and deep inherits " : (flags["r"] ? " and inherits " : "")) + "...\n", this_user());
    }

    switch (file_size(input)) {
        case -1:
            message("system", "update: " + input + " not found.\n", this_user());
            return;
        case 0:
            message("system", "update: " + input + " is empty.\n", this_user());
            return;
    }

    ob = find_object(input);

    if (flags["R"] || flags["r"]) {
        string *list;
        int i;

        if (!ob) {
            tmp = catch (ob = load_object(input));
            if (tmp) {
                message("system", "update failed: " + input + ":\n" + tmp + "\n", this_user());
                return;
            }
        }

        if (flags["R"]) {
            list = deep_inherit_list(ob);
        } else if (flags["r"]) {
            list = inherit_list(ob);
        }
        i = sizeof(list);
        while (i --) {
            object o = find_object(list[i]);
            if (o) {
                o->handle_remove();
            }
            if (o) {
                destruct(o);
            }
            tmp = catch (load_object(list[i]));

            if (!tmp) {
                // @TODO cleanup how testing on recursive updates work
                // string test = list[i][0..<2] + "test.c";
                message("system", "update: " + list[i] + ": Ok\n", this_user());
                // if (file_size(test) > 0) {
                //     testStart = utime()[0] + utime()[1];
                //     D_TEST->process_file(test, (: done :), 1);
                // }
            } else {
                message("system", "update failed: " + list[i] + ":\n" + tmp + "\n", this_user());
                return;
            }
        }
    }

    if (ob && ob->query_living_contents()) {
        foreach (object l in ob->query_living_contents()) {
            if (l->is_character()) {
                keep += ({ l });
                l->handle_move("/domain/Nowhere/room/void.c");
            }
        }
    }
    if (ob) {
        ob->handle_remove();
    }
    if (ob) {
        destruct(ob);
    }

    tmp = catch (load_object(input));
    if (!tmp) {
        string test = input[0..<2] + "test.c";
        message("system", "update: " + input + ": Ok\n", this_user());
        foreach (object l in keep) {
            l->handle_move(input);
        }
        keep->describe_environment();
        keep = ({});
        if (file_size(test) > 0) {
            testStart = utime()[0] + utime()[1];
            D_TEST->process_file(test, (: done :), 1);
        }
    } else {
        message("system", "update failed: " + input + ":\n" + tmp + "\n", this_user());
    }
}