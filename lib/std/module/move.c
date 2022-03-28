private string __EnvPath;
nosave private object __LastEnv, __CurrentEnv;

private object query_dest_ob (mixed dest) {
    object destOb;

    if (objectp(dest)) {
        destOb = dest;
    } else if (stringp(dest)) {
        if (!(destOb = find_object(dest))) {
            if (catch(load_object(dest)) ) {
                return 0;
            }
        }
    }
    return destOb;
}

int handle_move (mixed dest) {
    object destOb;
    int x;

    if (!this_object()) return 0;
    if (environment()) {
        x = environment()->can_release(this_object());
        if (!x /* && !archp(this_object()) */) return 0; // @TODO archp? assistp?
    }
    if (!(destOb = query_dest_ob(dest))) {
        error("Bad argument 1 to move->handle_move");
    }
    if (!destOb || destOb == this_object()) return 0;
    if (!destOb->can_receive(this_object())) return 0;
    if (__LastEnv = environment()) {
        environment()->handle_release(this_object());
    }
    move_object(destOb);
    if (!regexp(file_name(destOb), "^/domain/Nowhere/room/(freezer|void)$")) {
        __EnvPath = file_name(destOb);
        // if (!regexp(__EnvPath, "\.c$")) __EnvPath += ".c";
    }
    __CurrentEnv = environment();
    environment()->handle_receive(this_object());
    return __LastEnv != environment();
}

string query_environment_path () {
    if (!__EnvPath) {
        return "/domain/Nowhere/room/void";
    } else {
        return __EnvPath;
    }
}
string query_environment_short () {
    int shouldDestruct = 0;

    if (__CurrentEnv) {
        write("__CurrentEnv: "+identify(__CurrentEnv)+"\n");
        return __CurrentEnv->query_short();
    } else if (__EnvPath) {
        object r;
        string short;
        write("No __CurrentEnv, check __EnvPath: "+identify(__EnvPath)+"\n");

        if (!(r = find_object(__EnvPath))) {
            write("r still not loaded "+identify(r)+"\n");
            if (catch(r = load_object(__EnvPath)) ) {
                write("trying to load_object for some reason\n");
                return 0;
            }
            // if (r) shouldDestruct = 1;
        }
        write("r is "+identify(r)+"\n");

        short = r->query_short();
        // if (shouldDestruct) destruct(r);
        return short;
    } else {
        return "no where";
    }
}
void set_environment_path (mixed dest) {
    object destOb;

    if (!(destOb = query_dest_ob(dest))) {
        error("Bad argument 1 to move->set_environment_path");
    }

    __EnvPath = file_name(destOb);
    // if (!regexp(__EnvPath, "\.c$")) __EnvPath += ".c";
}

/*
// @TODO cleanup query_environment_path vs query_environment_short

// set_environment_path
done    /secure/user/character.c->character_enter, line 63
done    /secure/user/character.c->character_enter, line 65
/std/account.test.c->test_account_characters, line 127
done    /std/living.c->handle_move, line 108 (move this to /std/module/move.c)

// query_environment_path <- query_last_location
done    /std/character.c->enter_world, line 119
done    /std/character.c->exit_freezer, line 149

// query_environment_short <- query_last_location
/std/acccount.c->update_character_data, line 112
*/