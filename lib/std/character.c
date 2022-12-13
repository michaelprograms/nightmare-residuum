
inherit STD_LIVING;
inherit M_PARENT;
inherit M_PROPERTY;
inherit M_SAVE;
inherit "/std/character/autoload.c";
inherit "/std/character/channel.c";
inherit "/std/character/history.c";

#include "living/living.h";

private int __LastAction;
private int __Created = time();
private int __Immortal;
private int __ConnectionTime = 0;

nosave private object __User;

void describe_environment ();
int query_immortal ();

int is_character () { return 1; }

varargs void save_character (int exit) {
    update_autoload(exit);
    save_data();
    reset_autoload();
}
void restore_character () {
    restore_data();
    restore_autoload();
}

// -----------------------------------------------------------------------------

void create () {
    living::create();
    parent::create();
    autoload::create();
}

void heart_beat () {
    ::heart_beat();
    __ConnectionTime ++; // 1 second heartbeat

    if (__ConnectionTime % 60 == 0) { // autosave
        if (__User && __User->query_account()) {
            __User->query_account()->update_character_data(this_object());
        }
        save_character();
    }
}

string query_character_short () {
    string short = query_cap_name();

    if (query_immortal()) short += " the immortal";
    else short += " the character";

    return short;
}
string query_character_long () {
    string long = query_cap_name();

    if (query_immortal()) long += " the immortal.";
    else long += " the character.";

    return long;
}

void set_name (string name) {
    living::set_name(name);
    set_short((: query_character_short :));
    set_long((: query_character_long :));
    set_save_path(D_CHARACTER->query_save_path(query_key_name(), "character"));
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
    if (__Immortal) {
        if (file_size("/realm/" + query_key_name() + "/") == -1) {
            mkdir("/realm/" + query_key_name() + "/");
        }
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
int query_connection_time () {
    return __ConnectionTime;
}

void setup_character () {
    __LastAction = time();
    if (!D_CHARACTER->query_exists(query_key_name())) {
        set_level(1);
        D_SPECIES->setup_stats(this_object());
        update_vitals(1);
        save_character();
    } else {
        restore_character();
        update_vitals(0);
    }
    living::update_limbs();
}

varargs void enter_world (int override) {
    if (!override) {
        string err = catch (handle_move(query_environment_path()));
        if (err) {
            handle_move("/domain/Nowhere/room/void.c");
        }
        D_CHANNEL->send_system("connection", query_cap_name() + " enters " + mud_name() + ".");
        master()->handle_parse_refresh();
        message("action", query_cap_name() + " suddenly appears into existence.", environment(), this_object());
    }
    describe_environment();
    set_heart_beat(1);
}

void exit_world () {
    message("action", query_cap_name() + " suddenly fades from existence.", environment(), this_object());
    D_CHANNEL->send_system("connection", query_cap_name() + " exits " + mud_name() + ".");

    save_character(1);
    handle_remove();
    master()->handle_parse_refresh();
}

void enter_freezer () {
    message("action", query_cap_name()+" suddenly fades from existence.", environment(), this_object());
    handle_move("/domain/Nowhere/room/freezer.c");
    D_CHANNEL->send_system("connection", query_cap_name() + " disconnects from " + mud_name() + ".");
    set_heart_beat(0);
}

void exit_freezer () {
    handle_move(query_environment_path());
    D_CHANNEL->send_system("connection", query_cap_name() + " reconnects to " + mud_name() + ".");
    message("action", query_cap_name()+" suddenly appears into existence.", environment(), this_object());
    describe_environment();
    set_heart_beat(1);
}

/* ----- describe environments ---- */

void describe_environment_long () {
    object env = environment();
    string *map = ({ });
    int width = __User->query_account()->query_setting("width");

    if (__User->query_account()->query_setting("screenreader") == "off") {
        map = env->query_room_exit_map();
    }
    if (!sizeof(map)) {
        message("room", "%^BOLD%^" + env->query_short() + "%^RESET%^", this_object());
        message("room", env->query_long(), this_object());
    } else {
        string *long, result = "", line;
        int l;
        long = ({ "%^BOLD%^" + env->query_short() + "%^RESET%^" }) + explode(wrap_ansi(env->query_long(), width-16), "\n");

        map = map(map, (: $1+" " :));
        l = max(({ sizeof(map), sizeof(long) }));
        for (int i = 0; i < l; i ++) {
            if (i < sizeof(map)) {
                line = map[i];
            } else {
                line = sprintf("%*s", 16, "");
            }
            if (i < sizeof(long)) {
                line += long[i];
            }
            result += line + "\n";
        }
        message("system", result, this_object());
    }
}

varargs void describe_environment_senses (string sense, string focus) {
    mixed tmp;
    string result;

    if (!focus) {
        focus = "default";
    }

    if ((!sense || sense == "listen") && (tmp = environment()->query_listen(focus))) {
        if (functionp(tmp)) {
            result = evaluate(tmp);
        } else if (stringp(tmp)) {
            result = tmp;
        }
        message("room listen", result, this_object());
    }
    if ((!sense || sense == "smell") && (tmp = environment()->query_smell(focus))) {
        if (functionp(tmp)) {
            result = evaluate(tmp);
        } else if (stringp(tmp)) {
            result = tmp;
        }
        message("room smell", result, this_object());
    }
}

private void describe_environment_exits () {
    string *exits;
    int numExits;

    if (!(numExits = sizeof(exits = environment()->query_exit_dirs()))) {
        message("room exits", "There are no exits.\n", this_object());
    } else {
        exits = map_array(exits, (: "%^CYAN%^BOLD%^" + $1 + "%^BOLD_OFF%^DEFAULT%^" :));
        message("room exits", "\nThere " + (numExits > 1 ? "are" : "is") + " " + cardinal(numExits) + " exit" + (numExits > 1 ? "s" : "") + ": " + conjunction(exits) + ".\n", this_object());
    }
}

private string describe_living_item (object ob) {
    string tag = "";
    switch (ob->query_hp() * 100 / (ob->query_max_hp() + 1)) {
        case 72..95: tag = " (bruised)"; break;
        case 48..71: tag = " (injured)"; break;
        case 24..47: tag = " (bleeding)"; break;
        case 0..23:  tag = " (dying)"; break;
    }
    switch (ob->query_posture()) {
        case "flying": case "laying": case "meditating": case "sitting":
            tag += " ("+ob->query_posture()+")" break;
    }
    return ob->query_short("%^RED%^BOLD%^") + tag;
}

private void describe_environment_living_contents () {
    object env = environment(), *contents = env->query_living_contents();
    mixed *list;
    string *shorts, conjunctions;

    list = filter_array(contents, (: $1 != this_object() :));
    list = sort_array(list, function (object a, object b) {
        if (a->is_character()) {
            if (b->is_character()) return strcmp(a->query_cap_name(), b->query_cap_name());
            else return -1;
        } else if (b->is_character()) {
            if (a->is_character()) return strcmp(a->query_cap_name(), b->query_cap_name());
            else return 1;
        } else return strcmp(a->query_cap_name(), b->query_cap_name());
    });
    list = unique_array(list, (: describe_living_item :));
    if (sizeof(list)) {
        shorts = map_array(list, function (object *obs) {
            return consolidate(sizeof(obs), describe_living_item(obs[0]));
        });
        shorts = map_array(shorts, (: $1 :));
        shorts[0] = capitalize(shorts[0]);
        shorts = map_array(shorts, (: "%^BOLD%^" + $1 + "%^BOLD_OFF%^DEFAULT%^" :));
        conjunctions = conjunction(shorts);
        message("room living contents", conjunctions + " " + (sizeof(contents) > 1 ? "are" : "is") + " here.\n", this_object());
    }
}

private void describe_environment_item_contents () {
    object env = environment(), *contents = env->query_item_contents();
    mixed *list;
    string *shorts, conjunctions;

    list = unique_array(contents, (: $1->query_short("%^MAGENTA%^BOLD%^") :));
    if (sizeof(list)) {
        shorts = sort_array(map_array(list, (: consolidate(sizeof($1), $1[0]->query_short("%^MAGENTA%^BOLD%^")) :)), 1);
        shorts[0] = capitalize(shorts[0]);
        shorts = map_array(shorts, (: "%^BOLD%^" + $1 + "%^BOLD_OFF%^DEFAULT%^" :));
        conjunctions = conjunction(shorts);
        message("room item contents", conjunctions + " " + (sizeof(contents) > 1 ? "are" : "is") + " here.", this_object());
    }
}

void describe_environment () {
    object env;

    if (!(env = environment()) || !env->is_room()) {
        message("room", "You do not have an environment.", this_object());
        return;
    }

    if (query_immortal()) {
        message("room", "%^UNDERLINE%^" + file_name(env) + "%^RESET%^", this_object());
    }

    describe_environment_long();
    describe_environment_senses();
    describe_environment_exits();
    describe_environment_living_contents();
    describe_environment_item_contents();
}