void doneCallback (int numTests, int numPassed, int numFailed, int fnsTested, int fnsUntested) {
    // write(numTests+" tests, "+numPassed+" passed, "+numFailed+" failed, "+fnsTested+" fnsTested, "+fnsUntested+" fnsUntested\n");
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
        write("Attempting ob->handle_destruct()...\n");
        ob->handle_destruct();
    }
    if (ob) {
        write("Attempting destruct(ob)...\n");
        destruct(ob);
    }

    tmp = catch (call_other(input, "???"));
    if (!tmp) {
        mapping tests = ([
            0: input[0..<2] + "test.c"
        ]);
        message("system", input + ": Ok\n", this_user());
        foreach (object l in keep) {
            l->handle_move(input);
        }
        keep = ({});
        if (file_size(tests[0]) > 0) {
            message("system", "\n", this_user());
            // this_user()->query_character()->set_property("updating", 1);
            // write("properties: "+identify(this_user()->query_character()->query_properties())+"\n");
            if (find_object(tests[0])) {
                catch(destruct(find_object(tests[0])));
            }
            call_other(tests[0], "???");
            call_out(function(mapping tests) {
                tests[0]->execute_test((: doneCallback:));
            }, 0, tests);
            // this_user()->query_character()->set_property("updating", 0);
            // write("properties: "+identify(this_user()->query_character()->query_properties())+"\n");
        }
    } else message("system", input + ": Error in update\n" + tmp+"\n", this_user());
}