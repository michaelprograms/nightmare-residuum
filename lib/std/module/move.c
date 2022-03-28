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
    if (__CurrentEnv) {
        return __CurrentEnv->query_short();
    } else if (__EnvPath) {
        object r;
        string short;

        if (!(r = find_object(__EnvPath))) {
            if (catch(r = load_object(__EnvPath)) ) { // @TODO will this memory leak?
                return 0;
            }
        }

        short = r->query_short();
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
}