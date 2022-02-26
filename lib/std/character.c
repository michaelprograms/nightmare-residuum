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

void create () {
    living::create();
    parent::create();
}

void set_name (string name) {
    living::set_name(name);
    set_short(name);
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
        D_CHANNEL->send_system("connection", query_name() + " connects.");
        message("connection", query_name()+" enters "+mud_name()+".\n", environment(this_object()), this_object());
    }
    describe_environment();
    set_heart_beat(1);
}

void exit_world () {
    save_data();
    message("connection", query_name()+" exits "+mud_name()+".\n", environment(this_object()), this_object());
    D_CHANNEL->send_system("connection", query_name() + " exits.");
    call_out((: master()->handle_parse_refresh() :), 0);
    handle_remove();
}

void enter_freezer () {
    message("connection", query_name()+" suddenly fades from existence.\n", environment(this_object()), this_object());
    handle_move("/domain/Nowhere/freezer.c");
    D_CHANNEL->send_system("connection", query_name() + " disconnects.");
    set_heart_beat(0);
}

void exit_freezer () {
    handle_move(query_last_environment());
    D_CHANNEL->send_system("connection", query_name() + " reconnects.");
    message("connection", query_name()+" suddenly appears into existence.\n", environment(this_object()), this_object());
    describe_environment();
    set_heart_beat(1);
}

// -----------------------------------------------------------------------------

private void describe_environment_living_contents () {
    object env = environment();
    mixed *list;
    string *shorts;

    if (!env || !env->is_room()) return;

    list = filter_array(env->query_living_contents(), (: $1 != this_object() :));
    list = unique_array(list, (: $1->query_short() :));
    if (sizeof(list)) {
        shorts = sort_array(map_array(list, (: capitalize(consolidate(sizeof($1), $1[0]->query_short())) :)), 1);
        shorts = map_array(shorts, (: $1 :));
        message("room_living_contents", implode(shorts, "\n") + "\n\n", this_object());
    }
}

private void describe_environment_item_contents () {
    object env = environment();
    mixed *list;
    string *shorts, conjunctions;

    if (!env || !env->is_room()) return;

    list = unique_array(env->query_item_contents(), (: $1->query_short() :));
    if (sizeof(list)) {
        shorts = sort_array(map_array(list, (: consolidate(sizeof($1), $1[0]->query_short()) :)), 1);
        shorts[0] = capitalize(shorts[0]);
        shorts = map_array(shorts, (: "%^BOLD%^" + $1 + "%^BOLD_OFF%^DEFAULT%^" :));
        conjunctions = conjunction(shorts);
        message("room_item_contents", conjunctions + " " + (regexp(conjunctions, "and") ? "are" : "is") + " here.\n", this_object());
    }
}

private void describe_environment_exits () {
    object env = environment();
    string *exits;
    int numExits;

    if (!(numExits = sizeof(exits = env->query_exit_dirs()))) {
        message("room_exits", "There are no exits.\n\n", this_object());
    } else {
        exits = map_array(exits, (: "%^CYAN%^BOLD%^" + $1 + "%^BOLD_OFF%^DEFAULT%^" :));
        message("room_exits", "There " + (numExits > 1 ? "are" : "is") + " " + cardinal(numExits) + " exit" + (numExits > 1 ? "s" : "") + ": " + conjunction(exits) + ".\n\n", this_object());
    }
}

void describe_environment () {
    object env;
    string *map;

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
    describe_environment_exits();
    describe_environment_living_contents();
    describe_environment_item_contents();
}

string *query_channels () {
    return ({ "chat" }); // @TODO
}