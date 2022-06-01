private string __EnvPath;
nosave private object __LastEnv, __CurrentEnv;

private object query_dest_ob (mixed dest) {
    object destOb;

    if (objectp(dest)) {
        destOb = dest;
    } else if (stringp(dest)) {
        if (!(destOb = find_object(dest))) {
            if (catch(destOb = load_object(dest)) ) {
                return 0;
            }
        }
    }
    return destOb;
}

void handle_released (object env) {

}

void handle_received (object env) {

}

int handle_move (mixed dest) {
    object destOb;

    if (!this_object()) return 0;
    if (environment()) {
        int release = environment()->can_release(this_object());
        if (!release && !this_object()->query_immortal()) return 0;
    }
    if (!(destOb = query_dest_ob(dest))) {
        raise_error("Bad argument 1 to move->handle_move");
    }
    if (!destOb || destOb == this_object()) return 0;
    if (!destOb->can_receive(this_object())) return 0;
    if (__LastEnv = environment()) {
        environment()->handle_release(this_object());
        handle_released(environment());
    }
    move_object(this_object(), destOb);
    if (!sizeof(regexp(({ program_name(destOb) }), "^/domain/Nowhere/room/(freezer|void)$"))) {
        __EnvPath = program_name(destOb);
    }
    __CurrentEnv = environment();
    environment()->handle_receive(this_object());
    handle_received(environment());
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
        raise_error("Bad argument 1 to move->set_environment_path");
    }

    __EnvPath = program_name(destOb);
}