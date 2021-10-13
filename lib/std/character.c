inherit STD_LIVING;
inherit M_SAVE;

private int __LastAction;
private int __Created = time();
nosave private object __User;

void describe_environment ();

int is_character () { return 1; }

// -----------------------------------------------------------------------------

void create() {
    living::create();
    set_no_clean(1);
}

void set_name (string name) {
    living::set_name(name);
    set_save_path(D_CHARACTER->query_save_path(query_key_name()));
}

// -----------------------------------------------------------------------------

void receive_message (string type, string message) {
    if (__User) {
        __User->receive_message(type, message);
    }
}

object query_user () {
    return __User;
}
void set_user (object user) {
    // @TODO security?
    __User = user;
}
void update_last_action () { // @TODO set_last_action?
    __LastAction = time();
}
int query_last_action () {
    return __LastAction;
}

void setup_character () {
    __LastAction = time();
    if (!D_CHARACTER->query_exists(query_key_name())) {
        save_data();
    } else {
        restore_data();
    }
}

void enter_world () {
    master()->handle_parse_refresh();
    if (query_last_environment()) {
        handle_move(query_last_environment());
    } else {
        handle_move("/domain/Nowhere/void.c");
    }
    message("connection", query_name()+" enters "+mud_name()+".\n", environment()->query_living_contents(), this_object());
    describe_environment();
}

void exit_world () {
    message("connection", query_name()+" exits "+mud_name()+".\n", environment()->query_living_contents(), this_object());
    save_data();
    master()->handle_parse_refresh();
    handle_remove();
}

void enter_freezer () {
    message("connection", query_name()+" suddenly fades from existence.\n", environment()->query_living_contents(), this_object());
    handle_move("/domain/Nowhere/freezer.c");
}

void exit_freezer () {
    handle_move(query_last_environment());
    message("connection", query_name()+" suddenly appears from existence.\n", environment()->query_living_contents(), this_object());
    describe_environment();
}

// -----------------------------------------------------------------------------

void describe_environment () {
    object env, *obs;
    string *exits;
    string *map;

    if (!(env = environment()) || !env->is_room()) {
        message("system", "You do not have an environment.\n", this_object());
        return;
    }

    if (map = env->query_map()) {
        message("room_map", implode(map, "\n") + "\n", this_object());
    }

    // @TODO creatorp?
    message("room_file", file_name(env)+"\n", this_object());
    message("room_short", env->query_short()+"\n", this_object());
    message("room_long", env->query_long()+"\n\n", this_object());

    if (!sizeof(exits = env->query_exit_dirs())) {
        message("room_exits", "There are no obvious exits visible.\n", this_object());
    } else {
        message("room_exits", "There are "+cardinal(sizeof(exits))+" exits: "+implode(map_array(exits, (:"%^CYAN%^"+$1+"%^RESET%^":)), ", ")+"\n", this_object());
    }

    if (sizeof(obs = filter_array(env->query_living_contents(), (:$1 != this_object():)))) {
        message("room_living_contents", "\n" + implode(map_array(obs, (:$1->query_name():)), "\n") + "\n", this_object());
    }
    if (sizeof(obs = env->query_nonliving_contents())) {
        message("room_nonliving_contents", "\n" + implode(map_array(obs, (:$1->query_name():)), "\n") + "\n", this_object());
    }
}