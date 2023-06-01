inherit STD_OBJECT;

inherit M_CLEAN;
inherit M_CONTAINER;
inherit M_EXIT;
inherit M_LOOK;
inherit M_PROPERTY;
inherit M_RESET;
inherit M_SENSES;

int is_room () { return 1; }

/* ----- applies ----- */
void create () {
    object::create();
    reset::create();
    senses::create();
    reset();

    if (this_object()) {
        D_ASTRONOMY->handle_room_create(this_object());
    }
}

void reset () {
    reset::reset();
}

/* ----- M_CLEAN ----- */

int clean_up (mixed *args...) {
    foreach (object ob in deep_inventory()) {
        if (ob->query_user() && userp(ob->query_user())) {
            return ::clean_later();
        }
    }
    return ::clean_up();
}

/* ----- M_CONTAINER ----- */

int handle_receive (object ob) {
    if (ob) {
        if (ob->is_living()) {
            object *obs = query_living_contents() + query_item_contents() - ({ ob });
            foreach (object o in obs) {
                // call out to delay fn til after move
                call_out_walltime(function (object ob, object o) {
                    if (objectp(ob) && objectp(o)) {
                        ob->handle_receive_living_in_env(o);
                        o->handle_receive_living_in_env(ob);
                    }
                }, 0, ob, o);
            }
        } else if (ob->is_item()) {
            object *obs = query_living_contents() + query_item_contents() - ({ ob });
            foreach (object o in obs) {
                // call out to delay fn til after move
                call_out_walltime(function (object ob, object o) {
                    if (objectp(ob) && objectp(o)) {
                        ob->handle_receive_item_in_env(o);
                        o->handle_receive_item_in_env(ob);
                    }
                }, 0, ob, o);
            }
        }
    }
    return ::handle_receive(ob);
}

int handle_release (object ob) {
    if (ob) {
        if (ob->is_living()) {
            object *obs = query_living_contents() + query_item_contents() - ({ ob });
            foreach (object o in obs) {
                o->handle_release_living_in_env(ob);
            }
        } else if (ob->is_item()) {
            object *obs = query_living_contents() + query_item_contents() - ({ ob });
            foreach (object o in obs) {
                o->handle_release_item_in_env(ob);
            }
        }
    }
    return ::handle_release(ob);
}

/* ----- room map ----- */

string query_room_map_format () {
    int aggressive = 0, passive = 0, item = 0;
    int cha;
    object *hostiles, char;
    string num = " ";

    char = this_character();
    cha = char->query_stat("charisma");
    hostiles = char->query_hostiles();

    foreach (object l in query_living_contents()) {
        if (cha < l->query_aggressive() || l->query_hostile(char)) {
            aggressive ++;
        } else {
            passive ++;
        }
    }

    if (aggressive || passive) {
        if (aggressive && passive) num = "%^ORANGE%^";
        if (aggressive && !passive) num = "%^BOLD%^RED%^";
        if (!aggressive && passive) num = "%^BOLD%^GREEN%^";

        num += (aggressive+passive > 10 ? "+" : ""+(aggressive+passive)) + "%^RESET%^";
    } else {
        item = sizeof(query_item_contents());
        if (item) num = "%^MAGENTA%^BOLD%^" + (item > 10 ? "+" : ""+item) + "%^RESET%^";
    }

    return "[" + num + "]";
}
mapping query_room_exits_picture () {
    mapping exits = query_exits();
    mapping b = query_border_charset();
    mapping picture = ([]);
    picture["nw"] = exits["northwest"] ? b["dd"] : " ";
    picture["n"]  = exits["north"]     ? b["v"]  : " ";
    picture["u"]  = exits["up"]        ? "+"  : " ";
    picture["ne"] = exits["northeast"] ? b["du"]  : " ";
    picture["w"]  = exits["west"]      ? b["h"]  : " ";
    picture["e"]  = exits["east"]      ? b["h"]  : " ";
    picture["sw"] = exits["southwest"] ? b["du"]  : " ";
    picture["d"]  = exits["down"]      ? "-"  : " ";
    picture["s"]  = exits["south"]     ? b["v"]  : " ";
    picture["se"] = exits["southeast"] ? b["dd"] : " ";
    return picture;
}
string *query_room_exit_map() {
    mapping blank, roomOb, exits, pics;

    if (query_property("no map")) {
        return 0;
    }

    blank = ([
        "nw": " ", "n":  " ", "ne": " ",
        "w":  " ", "u":  " ", "d":  " ", "e":  " ",
        "sw": " ", "s":  " ", "se": " ",
    ]);
    roomOb = ([
        "nw": 0, "n":  0, "ne": 0,
        "w":  0, "u":  0, "d":  0, "e":  0,
        "sw": 0, "s":  0, "se": 0,
    ]);
    exits = query_exits();
    pics = ([
        "nw": exits["northwest"] && file_size(exits["northwest"]["room"]) > 0 && (roomOb["nw"] = find_object(exits["northwest"]["room"],1)) ? roomOb["nw"]->query_room_exits_picture() : blank,
        "n":  exits["north"]     && file_size(exits["north"]["room"]) > 0 && (roomOb["n"] = find_object(exits["north"]["room"],1)) ? roomOb["n"]->query_room_exits_picture() : blank,
        "ne": exits["northeast"] && file_size(exits["northeast"]["room"]) > 0 && (roomOb["ne"] = find_object(exits["northeast"]["room"],1)) ? roomOb["ne"]->query_room_exits_picture() : blank,
        "w":  exits["west"]      && file_size(exits["west"]["room"]) > 0 && (roomOb["w"] = find_object(exits["west"]["room"],1)) ? roomOb["w"]->query_room_exits_picture() : blank,
        "x":  query_room_exits_picture(),
        "e":  exits["east"]      && file_size(exits["east"]["room"]) > 0 && (roomOb["e"] = find_object(exits["east"]["room"],1)) ? roomOb["e"]->query_room_exits_picture() : blank,
        "sw": exits["southwest"] && file_size(exits["southwest"]["room"]) > 0 && (roomOb["sw"] = find_object(exits["southwest"]["room"],1)) ? roomOb["sw"]->query_room_exits_picture() : blank,
        "s":  exits["south"]     && file_size(exits["south"]["room"]) > 0 && (roomOb["s"] = find_object(exits["south"]["room"],1)) ? roomOb["s"]->query_room_exits_picture() : blank,
        "se": exits["southeast"] && file_size(exits["southeast"]["room"]) > 0 && (roomOb["se"] = find_object(exits["southeast"]["room"],1)) ? roomOb["se"]->query_room_exits_picture() : blank,
    ]);
    // w-n
    // w-s
    // n-w
    // n-e
    // e-n
    // e-s
    // s-w
    // s-e
    // ne-w
    // ne-s
    // se-w
    // se-n
    // sw-n
    // sw-e
    // nw-s
    // nw-e
    return ({
        pics["nw"]["nw"] + " " + pics["nw"]["n"] + pics["nw"]["u"] + pics["nw"]["ne"] +
        pics["n"]["nw"]  + " " + pics["n"]["n"]  + pics["n"]["u"]  + pics["n"]["ne"]  +
        pics["ne"]["nw"] + " " + pics["ne"]["n"] + pics["ne"]["u"] + pics["ne"]["ne"],

        pics["nw"]["w"] + (roomOb["nw"] ? roomOb["nw"]->query_room_map_format() : "   ") + pics["nw"]["e"] +
        pics["n"]["w"]  + (roomOb["n"]  ? roomOb["n"]->query_room_map_format() : "   ") + pics["n"]["e"]  +
        pics["ne"]["w"] + (roomOb["ne"] ? roomOb["ne"]->query_room_map_format() : "   ") + pics["ne"]["e"],

        pics["nw"]["sw"] + pics["nw"]["d"] + pics["nw"]["s"] + " " + pics["nw"]["se"] +
        pics["n"]["sw"]  + pics["n"]["d"]  + pics["n"]["s"]  + " " + pics["n"]["se"]  +
        pics["ne"]["sw"] + pics["ne"]["d"] + pics["ne"]["s"] + " " + pics["ne"]["se"],

        // -----

        pics["w"]["nw"] + " " + pics["w"]["n"] + pics["w"]["u"] + pics["w"]["ne"] +
        pics["x"]["nw"] + " " + pics["x"]["n"] + pics["x"]["u"] + pics["x"]["ne"] +
        pics["e"]["nw"] + " " + pics["e"]["n"] + pics["e"]["u"] + pics["e"]["ne"],

        pics["w"]["w"] + (roomOb["w"] ? roomOb["w"]->query_room_map_format() : "   ") + pics["w"]["e"] +
        pics["x"]["w"] + "[%^CYAN%^BOLD%^X%^RESET%^]" + pics["x"]["e"] +
        pics["e"]["w"] + (roomOb["e"] ? roomOb["e"]->query_room_map_format() : "   ") + pics["e"]["e"],

        pics["w"]["sw"] + pics["w"]["d"] + pics["w"]["s"] + " " + pics["w"]["se"] +
        pics["x"]["sw"] + pics["x"]["d"] + pics["x"]["s"] + " " + pics["x"]["se"] +
        pics["e"]["sw"] + pics["e"]["d"] + pics["e"]["s"] + " " + pics["e"]["se"],

        // -----

        pics["sw"]["nw"] + " " + pics["sw"]["n"] + pics["sw"]["u"] + pics["sw"]["ne"] +
        pics["s"]["nw"]  + " " + pics["s"]["n"]  + pics["s"]["u"]  + pics["s"]["ne"]  +
        pics["se"]["nw"] + " " + pics["se"]["n"] + pics["se"]["u"] + pics["se"]["ne"],

        pics["sw"]["w"] + (roomOb["sw"] ? roomOb["sw"]->query_room_map_format() : "   ") + pics["sw"]["e"] +
        pics["s"]["w"]  + (roomOb["s"]  ? roomOb["s"]->query_room_map_format() : "   ") + pics["s"]["e"]  +
        pics["se"]["w"] + (roomOb["se"] ? roomOb["se"]->query_room_map_format() : "   ") + pics["se"]["e"],

        pics["sw"]["sw"] + pics["sw"]["d"] + pics["sw"]["s"] + " " + pics["sw"]["se"] +
        pics["s"]["sw"]  + pics["s"]["d"]  + pics["s"]["s"]  + " "  + pics["s"]["se"]  +
        pics["se"]["sw"] + pics["se"]["d"] + pics["se"]["s"] + " " + pics["se"]["se"],
    });
}