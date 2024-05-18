inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("update -b(rief) -c(overage) [file]");
    set_help_text("The update command is used to destruct and reload a file to the latest copy, and run tests of the file, if they exist.");
}

void command (string input, mapping flags) {
    string tmp;
    object ob;
    object *keep = ({ });

    if (!input || input == "") {
        message("action", "Update which file?", this_user());
        return;
    } if (input == "here") {
        input = file_name(environment(this_character()));
        if (file_size(input) == -1 && file_size(input + ".c") > 0) input += ".c";
    } else {
        if (input[0] != '/' && input[0] != '~' && input[0] != '^') {
            input = this_user()->query_variable("cwd") + "/" + input;
        }
        input = sanitize_path(input);
        message("action", "Updating " + input + (flags["R"] ? " and deep inherits " : (flags["r"] ? " and inherits " : "")) + "...", this_user());
    }

    switch (file_size(input)) {
        case -1:
            message("action", "update: " + input + " not found.", this_user());
            return;
        case 0:
            message("action", "update: " + input + " is empty.", this_user());
            return;
    }

    ob = find_object(input);

    if (flags["R"] || flags["r"]) {
        string *list;
        int i;

        if (!ob) {
            tmp = catch (ob = load_object(input));
            if (tmp) {
                message("action", "update failed: " + input + ":\n" + tmp, this_user());
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
                message("action", "update: " + list[i] + ": Ok", this_user());
                // if (file_size(test) > 0) {
                //     testStart = time_ns();
                //     D_TEST->process_file(test, ([ "reset": 1 ]));
                // }
            } else {
                message("action", "update failed: " + list[i] + ":\n" + tmp, this_user());
                return;
            }
        }
    }

    if (ob && ob->query_living_contents()) {
        foreach (object l in ob->query_living_contents()) {
            if (l->is_character()) {
                keep += ({ l });
                l->handle_move("/domain/Nowhere/room/void.c");
            } else {
                l->handle_remove();
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
        message("action", "update: " + input + ": Ok", this_user());
        foreach (object l in keep) {
            l->handle_move(input);
        }
        keep->describe_environment();
        keep = ({ });
        if (file_size(test) > 0) {
            D_TEST->process_file(test, ([
                "brief": flags["brief"] || flags["b"],
                "coverage": flags["coverage"] || flags["c"],
                "reset": 1
            ]));
        }
    } else {
        message("action", "update failed: " + input, this_user());
    }
}