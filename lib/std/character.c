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
void set_user (object account) {
    // @TODO security?
    __User = account;
}
void update_last_action () { // @TODO set_last_action?
    __LastAction = time();
}
int query_last_action () {
    return __LastAction;
}

void setup () {
    __LastAction = time();
    // write("__User: "+identify(__User)+"\n");
    if (!D_CHARACTER->query_exists(this_object()->query_key_name())) {
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
    describe_environment();
}

void exit_world () {
    save_data();
    master()->handle_parse_refresh();
    destruct(); // @TODO handle_destruct
}

// -----------------------------------------------------------------------------

void describe_environment () {
    object env, *obs;
    string *exits;

    if ((env = environment()) && env->is_room()) {
        tell(this_object(), env->query_short()+"\n");
        tell(this_object(), env->query_long()+"\n\n");

        if (!sizeof(exits = env->query_exit_directions())) {
            tell(this_object(), "There are no obvious exits visible.\n");
        } else {
            tell(this_object(), "There are "+cardinal(sizeof(exits))+" exits: "+implode(exits, ", ")+"\n");
        }

        if (sizeof(obs = filter_array(env->query_living_contents(), (:$1 != this_object():)))) {
            tell(this_object(), "\n" + implode(map_array(obs, (:$1->query_name():)), "\n") + "\n");
        }
        if (sizeof(obs = env->query_nonliving_contents())) {
            tell(this_object(), "\n" + implode(map_array(obs, (:$1->query_name():)), "\n") + "\n");
        }
    } else {
        tell(this_object(), "You do not have an environment.\n");
    }
}