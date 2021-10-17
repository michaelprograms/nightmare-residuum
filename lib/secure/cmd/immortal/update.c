varargs void done (int numTests, int numPassed, int numFailed, int fnsTested, int fnsUntested) {
    int numExpects = numPassed + numFailed;
    if (!numExpects) {
        message("system", "\nNo expects found.\n", this_user());
        return;
    }
    message("system", "\nPassed:    " + sprintf("%3d", numPassed) + " / " + sprintf("%3d", numExpects) + " (" + sprintf("%3d", to_int(numPassed * 100 / numExpects)) + "%)"+"\n", this_user());
    message("system", "Failed:    " + sprintf("%3d", numFailed) + " / " + sprintf("%3d", numExpects) + " (" + sprintf("%3d", to_int(numFailed * 100 / numExpects)) + "%)"+"\n", this_user());
    message("system", "Functions: " + sprintf("%3d", fnsTested) + " / " + sprintf("%3d", (fnsTested + fnsUntested)) + " (" + sprintf("%3d", to_int(fnsTested * 100 / (fnsTested + fnsUntested))) + "%)"+"\n\n", this_user());
}

void command (string input) {
    string tmp;
    object ob;
    object *keep = ({});

    if (!input) {
        write("Update which file?\n");
        return;
    } else {
        write("Updating "+input+"...\n");
    }

    // @TODO check file exists, check container contents
    ob = find_object(input);
    if (ob) {
        if (ob->is_room()) {
            foreach (object l in ob->query_living_contents()) {
                if (l->is_character()) {
                    keep += ({ l });
                    l->handle_move("/domain/Nowhere/void.c");
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

    tmp = catch (call_other(input, "???"));
    if (!tmp) {
        string test = input[0..<2] + "test.c";
        message("system", input + ": Ok\n", this_user());
        foreach (object l in keep) {
            l->handle_move(input);
        }
        keep = ({});
        if (file_size(test) > 0) {
            D_TEST->process_file(test, (: done :));
        }
    } else {
        message("system", input + ": Error in update\n" + tmp+"\n", this_user());
    }
}