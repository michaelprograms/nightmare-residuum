private string __EnvPath;
nosave private object __LastEnv;
/** @type {STD_STORAGE} __CurrentEnv */
nosave private object __CurrentEnv;

/**
 * Determine if the requested destination is a path or an object. When a path
 * is provided, turn it into an object.
 *
 * @param dest the destination to check
 * @returns {STD_STORAGE}
 */
object query_dest_ob (mixed dest) {
    object destOb;
    if (objectp(dest)) {
        destOb = dest;
    } else if (stringp(dest)) {
        if (!(destOb = find_object(dest)) && !(destOb = load_object(dest))) {
            return 0;
        }
    }
    return destOb;
}

/**
 * Called after an object has been moved to a new environment.
 *
 * @param {STD_STORAGE} env
 */
void handle_received (object env) {
    if (env) {
        env->handle_receive(this_object());
    }
}
/**
 * Called before an object has been moved to a new environment.
 *
 * @param {STD_STORAGE} env
 */
void handle_released (object env) {
    if (env) {
        env->handle_release(this_object());
    }
}

/**
 * Attempt to move an object from one environment to another environment.
 *
 * @param dest
 * @returns 0 or 1 for success
 */
int handle_move (mixed dest) {
    /** @type {STD_STORAGE} env */
    object env = environment();
    /** @type {STD_STORAGE} destOb */
    object destOb;
    if (env && !env->can_release(this_object()) && !this_object()->query_immortal()) {
        return 0;
    }
    if (!(destOb = query_dest_ob(dest))) {
        error("Bad argument 1 to move->handle_move");
    }
    if (!destOb->can_receive(this_object())) {
        return 0;
    }
    if (__LastEnv = env) {
        handle_released(__LastEnv);
    }
    move_object(destOb);
    if (!regexp(base_name(destOb), "^/domain/Nowhere/room/(freezer|void)$")) {
        __EnvPath = base_name(destOb) + ".c";
    }
    __CurrentEnv = environment();
    handle_received(__CurrentEnv);
    return __LastEnv != environment();
}

string query_environment_path () {
    if (!__EnvPath) {
        return "/domain/Nowhere/room/void.c";
    } else {
        return __EnvPath;
    }
}
string query_environment_short () {
    if (__CurrentEnv) {
        return __CurrentEnv->query_short();
    } else if (__EnvPath) {
        return query_dest_ob(__EnvPath)->query_short() || "no where";
    } else {
        return "no where";
    }
}
void set_environment_path (mixed dest) {
    object destOb;
    if (!(destOb = query_dest_ob(dest))) {
        error("Bad argument 1 to move->set_environment_path");
    }
    __EnvPath = base_name(destOb) + ".c";
}