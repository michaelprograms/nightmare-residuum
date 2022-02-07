#include "living/living.h";

inherit STD_LIVING;
inherit M_PARENT;
inherit M_SAVE;

private int __LastAction;
private int __Created = time();
private int __Immortal;
nosave private object __User;

void describe_environment ();

int is_character () { return 1; }

// -----------------------------------------------------------------------------

void create() {
    living::create();
    parent::create();
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

int query_immortal () {
    return __Immortal;
}
void set_immortal (int i) {
    // @TODO security?
    __Immortal = i;
    if (file_size("/realm/" + query_key_name() + "/") == -1) {
        mkdir("/realm/" + query_key_name() + "/");
    }
}

int query_created () {
    return __Created;
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
        update_vitals(1);
    } else {
        restore_data();
        update_vitals(0);
    }
}

varargs void enter_world (int override) {
    master()->handle_parse_refresh();
    if (!override) {
        handle_move(query_last_environment() || "/domain/Nowhere/void.c");
        message("connection", query_name()+" enters "+mud_name()+".\n", environment()->query_living_contents(), this_object());
    }
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

private mapping group_contents (object *obs) {
    mapping list = ([]);
    string tmp;
    int i;

    if (i = sizeof(obs)) {
        while (i --) {
            if (!(tmp = obs[i]->query_short())) {
                tmp = obs[i]->query_name();
            }
            if (!tmp) continue;
            if (!list[tmp]) list[tmp] = ({});
            list[tmp] += ({ obs[i] });
        }
    }
    return list;
}

private void describe_living_contents () {
    object env = environment();
    mapping list;
    string *shorts, result = "";
    int i, x;

    if (!env || !env->is_room()) return;

    list = group_contents(filter_array(env->query_living_contents(), (: $1 != this_object() :)));

    i = sizeof(shorts = keys(list));
    while (i --) {
        if((x = sizeof(list[shorts[i]])) < 2) {
            result = sprintf("%s%s\n", result, capitalize(shorts[i]));
        } else {
            result = sprintf("%s%s\n", result, capitalize(consolidate(x, shorts[i])));
        }
    }
    if (strlen(result) > 0) {
        message("room_living_contents", result + "\n", this_object());
    }

}

private void describe_item_contents () {
    object env = environment();
    mapping list;
    string *shorts, result = "";
    int i, x;

    if (!env || !env->is_room()) return;

    list = group_contents(env->query_item_contents());

    if (!(i = sizeof(shorts = keys(list)))) {
        return;
    }

    if ((x = sizeof(list[shorts[-- i]])) == 1) {
        result = "%^BOLD%^" + capitalize(shorts[i]) + "%^BOLD_OFF%^";
    } else {
        result = "%^BOLD%^" + capitalize(consolidate(x, shorts[i])) + "%^BOLD_OFF%^";
    }
    if (!i) {
        result = sprintf("%s %s here.", result + "%^DEFAULT%^", (x < 2 ? "is" : "are"));
    } else if(i == 1) {
        result = sprintf("%s and %s are here.", result + "%^DEFAULT%^", "%^BOLD%^" + consolidate(sizeof(list[shorts[0]]), shorts[0]) + "%^BOLD_OFF%^DEFAULT%^");
    } else {
        while (i --) {
            if (!i) result = sprintf("%s, and ", result);
            else result = sprintf("%s, ", result);
            result = sprintf("%s%s", result, "%^BOLD%^" + consolidate(sizeof(list[shorts[i]]), shorts[i]) + "%^BOLD_OFF%^");
        }
        result = sprintf("%s are here.", result + "%^DEFAULT%^");
    }
    if (strlen(result) > 0) {
        message("room_item_contents", result + "\n", this_object());
    }
}

void describe_environment () {
    object env;
    string *map, *exits;

    if (!(env = environment()) || !env->is_room()) {
        message("system", "You do not have an environment.\n", this_object());
        return;
    }

    if (map = env->query_map()) {
        message("room_map", implode(map, "\n") + "\n", this_object());
    }

    if (query_immortal()) {
        message("room_file", file_name(env)+"\n", this_object());
    }
    message("room_short", env->query_short()+"\n", this_object());
    message("room_long", env->query_long()+"\n\n", this_object());

    if (!sizeof(exits = env->query_exit_dirs())) {
        message("room_exits", "There are no exits visible.\n\n", this_object());
    } else {
        message("room_exits", "There are " + cardinal(sizeof(exits) ) +" exits: " + implode(map_array(exits, (: "%^CYAN%^" + $1 + "%^RESET%^" :)), ", ") + "\n\n", this_object());
    }

    describe_living_contents();
    describe_item_contents();
}