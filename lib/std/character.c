inherit STD_LIVING;
inherit M_PROPERTY;
inherit M_SAVE;

inherit "/std/character/achievement.c";
inherit "/std/character/autoload.c";
inherit "/std/character/channel.c";
inherit "/std/character/history.c";

private string __Account;
private int __LastAction;
private int __Immortal;
private int __ConnectionTime = 0;

/** @type {STD_USER} */
nosave private object __User;

void describe_environment ();
int query_immortal ();

int is_character () {
    return 1;
}
int query_immortal () {
    return __Immortal;
}
void set_immortal (int i) {
    __Immortal = i;
    if (__Immortal && query_key_name() && file_size("/realm/" + query_key_name() + "/") == -1) {
        mkdir("/realm/" + query_key_name() + "/");
    }
}

/* ----- gmcp ----- */

void gmcp_update_vitals () {
    if (!__User) {
        return;
    }
    __User->gmcp_send_update("Char.Vitals", ([
        "hp": query_hp(),
        "maxhp": query_max_hp(),
        "sp": query_sp(),
        "maxsp": query_max_sp(),
        "mp": query_mp(),
        "maxmp": query_max_mp(),
    ]));
}

/* ----- account ----- */

void set_account (string account) {
    __Account = account;
}
string query_account () {
    return __Account;
}

/* ----- save ----- */

varargs void save_character (int exit) {
    update_autoload(exit);
    if (exit) {
        remove_properties();
    }
    save_data();
    reset_autoload();
    if (__User) {
        __User->update_character_data(this_object());
    }
}
void restore_character () {
    restore_data();
    restore_autoload();
}

/* ----- applies ----- */

void create () {
    living::create();
    autoload::create();
}

void heart_beat () {
    ::heart_beat();
    __ConnectionTime ++; // 1 second heartbeat

    if (__ConnectionTime % 60 == 0) { // autosave
        save_character(0);
    }
}

void receive_message (string type, string message) {
    if (__User) {
        __User->receive_message(type, message);
    }
}

/* ----- overrides ----- */

string query_character_short () {
    string short = query_cap_name();

    if (query_immortal()) {
        short += " the immortal";
    } else {
        short += " the character";
    }

    return short;
}

void set_name (string name) {
    living::set_name(name);
    set_short((: query_character_short :));
    set_save_path(D_CHARACTER->query_save_path(query_key_name(), "character"));
}

void add_hp (int n) {
    living::add_hp(n);
    gmcp_update_vitals();
}
void add_sp (int n) {
    living::add_sp(n);
    gmcp_update_vitals();
}
void add_mp (int n) {
    living::add_mp(n);
    gmcp_update_vitals();
}

void set_hp (int n) {
    living::set_hp(n);
    gmcp_update_vitals();
}
void set_sp (int n) {
    living::set_sp(n);
    gmcp_update_vitals();
}
void set_mp (int n) {
    living::set_mp(n);
    gmcp_update_vitals();
}

varargs int handle_go (mixed dest, string verb, string dir, string reverse) {
    int go = living::handle_go(dest, verb, dir, reverse);
    if (go) {
        describe_environment();
    }
    return go;
}

/* ----- user ----- */

/**
 * The user associated to this character.
 *
 * @returns {STD_USER} the user account
 */
object query_user () {
    return __User;
}
void set_user (object user) {
    __User = user;
}

/* ----- character ----- */

void set_last_action () {
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
        save_character(0);
    } else {
        restore_character();
        update_vitals(0);
    }
    living::update_limbs();
}

/* ----- connection ----- */

varargs void enter_world (int override) {
    if (!override) {
        string err = catch (handle_move(query_environment_path()));
        if (err) {
            handle_move("/domain/Nowhere/room/void.c");
        }
        D_LOG->log("character/enter", ctime() + " " + query_name() + " enters from " + query_ip_number());
        D_CHANNEL->send_system("connection", query_cap_name() + " enters " + mud_name() + ".");
        message("action", query_cap_name() + " suddenly appears into existence.", environment(), this_object());
        master()->handle_parse_refresh();
    }
    describe_environment();
    set_heart_beat(1);
    gmcp_update_vitals();
}

void exit_world () {
    D_LOG->log("character/enter", ctime() + " " + query_name() + " exits from " + query_ip_number());
    message("action", query_cap_name() + " suddenly fades from existence.", environment(), this_object());
    D_CHANNEL->send_system("connection", query_cap_name() + " exits " + mud_name() + ".");
    save_character(1);
    handle_remove();
    master()->handle_parse_refresh();
}

void enter_freezer () {
    D_LOG->log("character/enter", ctime() + " " + query_name() + " disconnects from " + query_ip_number());
    message("action", query_cap_name()+" suddenly fades from existence.", environment(), this_object());
    handle_move("/domain/Nowhere/room/freezer.c");
    D_CHANNEL->send_system("connection", query_cap_name() + " disconnects from " + mud_name() + ".");
    set_heart_beat(0);
}

void exit_freezer () {
    D_LOG->log("character/enter", ctime() + " " + query_name() + " reconnects from " + query_ip_number());
    handle_move(query_environment_path());
    D_CHANNEL->send_system("connection", query_cap_name() + " reconnects to " + mud_name() + ".");
    message("action", query_cap_name()+" suddenly appears into existence.", environment(), this_object());
    describe_environment();
    set_heart_beat(1);
    gmcp_update_vitals();
}

/* ----- describe environments ---- */

void describe_environment_long () {
    /** @type {STD_ROOM} env */
    object env = environment();
    string *map = ({ });
    int width = __User->query_setting("width");
    int sm, sl, l;
    string *long, result = "", line;

    if (__User->query_setting("screenreader") == "off") {
        map = env->query_room_map();
    }
    if (!(sm = sizeof(map))) {
        message("room", "%^I_WHITE%^BOLD%^" + env->query_short() + "%^RESET%^", this_object());
        message("room", env->query_long(), this_object());
    } else {
        long = ({ "%^I_WHITE%^BOLD%^" + env->query_short() + "%^RESET%^" }) + explode(wrap_ansi(env->query_long(), width-16), "\n");
        sl = sizeof(long);
        map = map(map, (: $1+" " :));
        l = max(({ sm, sl }));
        for (int i = 0; i < l; i ++) {
            if (i < sm) {
                line = map[i];
            } else {
                line = sprintf("%*s", 16, "");
            }
            if (i < sl) {
                line += long[i];
            }
            result += line + "\n";
        }
        message("system", result, this_object());
    }
    if (line = env->query_long_footer()) {
        message("room", line, this_object());
    }
}

varargs void describe_environment_senses (string sense, string focus) {
    /** @type {STD_ROOM} env */
    object env = environment();
    mixed tmp;
    string result;

    if (!focus) {
        focus = "default";
    }

    if ((!sense || sense == "listen") && (tmp = env->query_listen(focus))) {
        if (functionp(tmp)) {
            result = evaluate(tmp);
        } else if (stringp(tmp)) {
            result = tmp;
        }
        message("room listen", result, this_object());
    }
    result = "";
    if ((!sense || sense == "smell") && (tmp = env->query_smell(focus))) {
        if (functionp(tmp)) {
            result = evaluate(tmp);
        } else if (stringp(tmp)) {
            result = tmp;
        }
        message("room smell", result, this_object());
    }

    if (result) {
        message("system", "\n", this_object());
    }
}

private void describe_environment_exits () {
    /** @type {STD_ROOM} env */
    object env = environment();
    string *exits;
    int numExits;

    if (!env->query_hidden_exits()) {
        if (!(numExits = sizeof(exits = env->query_exit_directions()))) {
            message("room exits", "There are no exits.\n", this_object());
        } else {
            exits = map(
                exits,
                /**
                 * Format open/closed status if a door exists this direction.
                 * @param dir the direction being queried
                 * @param {STD_ROOM} env the room to check
                 * @returns the formatted door description
                 */
                function (string dir, object env) {
                    string door = env->query_dir_door(dir);
                    int open;
                    if (door) {
                        open = env->query_open(door);
                        door = " " + (!open ? "[" : "(") + door + (!open ? "]" : ")");
                    }
                    return "%^I_CYAN%^BOLD%^" + dir + "%^BOLD_OFF%^" + (door ? door : "") + "%^DEFAULT%^";
                },
                env
            );
            message("room exits", "There " + (numExits > 1 ? "are" : "is") + " " + cardinal(numExits) + " exit" + (numExits > 1 ? "s" : "") + ": " + conjunction(exits) + ".\n", this_object());
        }
    }
}

private void describe_environment_living_contents () {
    /** @type {STD_ROOM} env */
    object env = environment();
    object *contents = env->query_living_contents();
    mixed *list;
    string *shorts;

    list = filter(contents, (: $1 != this_object() :));
    list = sort_array(
        list,
        /**
         * Sort the list of living objects, with characters alphabetically
         * followed by NPCs alphabetically.
         * @param {STD_LIVING} a
         * @param {STD_LIVING} b
         */
        function (object a, object b) {
            if (characterp(a) && characterp(b)) {
                return strcmp(a->query_cap_name(), b->query_cap_name());
            } else if (characterp(a)) {
                return -1;
            } else if (characterp(b)) {
                return 1;
            } else {
                return strcmp(a->query_cap_name(), b->query_cap_name());
            }
        }
    );
    list = unique_array(list, (: describe_living_item :));
    if (sizeof(list)) {
        shorts = map(list, function (object *obs) {
            return consolidate(sizeof(obs), describe_living_item(obs[0]));
        });
        shorts = map(shorts, (: $1 :));
        shorts[0] = capitalize(shorts[0]);
        shorts = map(shorts, (: "%^I_RED%^BOLD%^" + $1 + "%^DEFAULT%^" :));
        message("room living contents", conjunction(shorts) + " " + (sizeof(shorts) > 1 ? "are" : "is") + " here.\n", this_object());
    }
}

private void describe_environment_item_contents () {
    /** @type {STD_ROOM} env */
    object env = environment();
    object *contents = env->query_item_contents();
    mixed *list;
    string *shorts;

    list = unique_array(contents, (: $1->query_short("%^I_MAGENTA%^BOLD%^") :));
    if (sizeof(list)) {
        shorts = sort_array(map(list, (: consolidate(sizeof($1), $1[0]->query_short("%^I_MAGENTA%^BOLD%^")) :)), 1);
        shorts[0] = capitalize(shorts[0]);
        shorts = map(shorts, (: "%^I_MAGENTA%^" + $1 + "%^DEFAULT%^" :));
        message("room item contents", conjunction(shorts) + " " + (sizeof(shorts) > 1 ? "are" : "is") + " here.", this_object());
    }
}

/**
 * Add extra description text for immortals.
 *
 * @param {STD_ROOM} env
 */
void describe_environment_immortal (object env) {
    string *props = ({ });
    foreach (string key,mixed value in env->query_properties()) {
        props += ({ key+": "+value });
    }
    message("room", "%^UNDERLINE%^" + file_name(env) + "%^RESET%^ " + implode(props, ", "), this_object());
}

void describe_environment () {
    /** @type {STD_ROOM} env */
    object env = environment();

    if (!(env = environment()) || !env->is_room()) {
        message("room", "You do not have an environment.", this_object());
        return;
    }

    query_immortal() && describe_environment_immortal(env);
    describe_environment_long();
    describe_environment_senses();
    describe_environment_exits();
    describe_environment_living_contents();
    describe_environment_item_contents();
}