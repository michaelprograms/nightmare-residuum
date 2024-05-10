inherit STD_OBJECT;

inherit M_CLEAN;
inherit M_CONTAINER;
inherit M_EXIT;
inherit M_LOOK;
inherit M_PROPERTY;
inherit M_RESET;
inherit M_SENSES;

int is_room () {
    return 1;
}

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
    return ::clean_up(args);
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

nosave private string roomBracketColor = "";
void set_room_bracket_color (string color) {
    roomBracketColor = color;
}
string query_room_bracket_color () {
    return roomBracketColor;
}
nosave private string *roomBrackets = ({ "[", "]" });
void set_room_brackets (string *brackets) {
    if (!arrayp(brackets) || sizeof(brackets) != 2) {
        error("Bad argument 1 to room->set_room_brackets");
    }
    roomBrackets = brackets;
}
string *query_room_brackets () {
    return roomBrackets;
}

string query_room_map_symbol () {
    int aggressive = 0, passive = 0, item = 0;
    int cha;
    object char = 0;
    object *hostiles = ({ });
    string symbol = " ";

    char = this_character();
    if (char) {
        cha = char->query_stat("charisma");
        hostiles = char->query_hostiles();
    }

    foreach (object l in query_living_contents()) {
        if (char && (cha < l->query_aggressive() || l->query_hostile(char))) {
            aggressive ++;
        } else {
            passive ++;
        }
    }

    if (aggressive || passive) {
        if (aggressive && passive) {
            symbol = "%^ORANGE%^";
        }
        if (aggressive && !passive) {
            symbol = "%^BOLD%^RED%^";
        }
        if (!aggressive && passive) {
            symbol = "%^BOLD%^GREEN%^";
        }

        symbol += (aggressive+passive > 10 ? "+" : ""+(aggressive+passive)) + "%^RESET%^";
    } else {
        item = sizeof(query_item_contents());
        if (item) {
            symbol = "%^MAGENTA%^BOLD%^" + (item > 10 ? "+" : ""+item) + "%^RESET%^";
        }
    }

    return symbol;
}

varargs string query_room_map_bracket (string symbol) {
    return roomBracketColor + roomBrackets[0] + "%^RESET%^" +
        (symbol ? symbol : query_room_map_symbol()) +
        roomBracketColor + roomBrackets[1] + "%^RESET%^";
}
varargs mapping query_room_exits_picture (string source) {
    mapping exits = query_exits();
    mapping b = query_border_charset();
    mapping picture = ([ ]);
    picture["nw"] = sizeof(exits["northwest"]) && (!source || (source && source == exits["northwest"]["room"])) ? b["dd"] : " ";
    picture["n"]  = sizeof(exits["north"])     && (!source || (source && source == exits["north"]["room"]))     ? b["v"] : " ";
    picture["u"]  = sizeof(exits["up"])        && (!source || (source && source == exits["up"]["room"]))        ? "+" : " ";
    picture["ne"] = sizeof(exits["northeast"]) && (!source || (source && source == exits["northeast"]["room"])) ? b["du"] : " ";
    picture["w"]  = sizeof(exits["west"])      && (!source || (source && source == exits["west"]["room"]))      ? b["h"] : " ";
    picture["e"]  = sizeof(exits["east"])      && (!source || (source && source == exits["east"]["room"]))      ? b["h"] : " ";
    picture["sw"] = sizeof(exits["southwest"]) && (!source || (source && source == exits["southwest"]["room"])) ? b["du"]  : " ";
    picture["d"]  = sizeof(exits["down"])      && (!source || (source && source == exits["down"]["room"]))      ? "-" : " ";
    picture["s"]  = sizeof(exits["south"])     && (!source || (source && source == exits["south"]["room"]))     ? b["v"] : " ";
    picture["se"] = sizeof(exits["southeast"]) && (!source || (source && source == exits["southeast"]["room"])) ? b["dd"] : " ";
    return picture;
}
private int valid_exit (string path) {
    if (regexp(path, "/virtual/")) {
        return 1;
    } else {
        return file_size(path) > 0;
    }
}
string *query_room_map () {
    mapping blank, roomOb, exits, pics;
    string source;

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
    source = file_name() + ".c";
    pics = ([
        "nw": exits["northwest"] && valid_exit(exits["northwest"]["room"]) > 0 && (roomOb["nw"] = load_object(exits["northwest"]["room"])) ? roomOb["nw"]->query_room_exits_picture(source) : blank,
        "n":  exits["north"]     && valid_exit(exits["north"]["room"]) > 0 && (roomOb["n"] = load_object(exits["north"]["room"])) ? roomOb["n"]->query_room_exits_picture(source) : blank,
        "ne": exits["northeast"] && valid_exit(exits["northeast"]["room"]) > 0 && (roomOb["ne"] = load_object(exits["northeast"]["room"])) ? roomOb["ne"]->query_room_exits_picture(source) : blank,
        "w":  exits["west"]      && valid_exit(exits["west"]["room"]) > 0 && (roomOb["w"] = load_object(exits["west"]["room"])) ? roomOb["w"]->query_room_exits_picture(source) : blank,
        "x":  query_room_exits_picture(),
        "e":  exits["east"]      && valid_exit(exits["east"]["room"]) > 0 && (roomOb["e"] = load_object(exits["east"]["room"])) ? roomOb["e"]->query_room_exits_picture(source) : blank,
        "sw": exits["southwest"] && valid_exit(exits["southwest"]["room"]) > 0 && (roomOb["sw"] = load_object(exits["southwest"]["room"])) ? roomOb["sw"]->query_room_exits_picture(source) : blank,
        "s":  exits["south"]     && valid_exit(exits["south"]["room"]) > 0 && (roomOb["s"] = load_object(exits["south"]["room"])) ? roomOb["s"]->query_room_exits_picture(source) : blank,
        "se": exits["southeast"] && valid_exit(exits["southeast"]["room"]) > 0 && (roomOb["se"] = load_object(exits["southeast"]["room"])) ? roomOb["se"]->query_room_exits_picture(source) : blank,
    ]);

    return ({
        pics["nw"]["nw"] + " " + pics["nw"]["n"] + pics["nw"]["u"] + pics["nw"]["ne"] +
        pics["n"]["nw"]  + " " + pics["n"]["n"]  + pics["n"]["u"]  + pics["n"]["ne"]  +
        pics["ne"]["nw"] + " " + pics["ne"]["n"] + pics["ne"]["u"] + pics["ne"]["ne"],

        pics["nw"]["w"] + (roomOb["nw"] ? roomOb["nw"]->query_room_map_bracket() : "   ") + pics["nw"]["e"] +
        pics["n"]["w"]  + (roomOb["n"]  ? roomOb["n"]->query_room_map_bracket() : "   ") + pics["n"]["e"]  +
        pics["ne"]["w"] + (roomOb["ne"] ? roomOb["ne"]->query_room_map_bracket() : "   ") + pics["ne"]["e"],

        pics["nw"]["sw"] + pics["nw"]["d"] + pics["nw"]["s"] + " " + pics["nw"]["se"] +
        pics["n"]["sw"]  + pics["n"]["d"]  + pics["n"]["s"]  + " " + pics["n"]["se"]  +
        pics["ne"]["sw"] + pics["ne"]["d"] + pics["ne"]["s"] + " " + pics["ne"]["se"],

        // -----

        pics["w"]["nw"] + " " + pics["w"]["n"] + pics["w"]["u"] + pics["w"]["ne"] +
        pics["x"]["nw"] + " " + pics["x"]["n"] + pics["x"]["u"] + pics["x"]["ne"] +
        pics["e"]["nw"] + " " + pics["e"]["n"] + pics["e"]["u"] + pics["e"]["ne"],

        pics["w"]["w"] + (roomOb["w"] ? roomOb["w"]->query_room_map_bracket() : "   ") + pics["w"]["e"] +
        pics["x"]["w"] + roomBracketColor + "[%^RESET%^CYAN%^BOLD%^X%^RESET%^" + roomBracketColor + "]%^RESET%^" + pics["x"]["e"] +
        pics["e"]["w"] + (roomOb["e"] ? roomOb["e"]->query_room_map_bracket() : "   ") + pics["e"]["e"],

        pics["w"]["sw"] + pics["w"]["d"] + pics["w"]["s"] + " " + pics["w"]["se"] +
        pics["x"]["sw"] + pics["x"]["d"] + pics["x"]["s"] + " " + pics["x"]["se"] +
        pics["e"]["sw"] + pics["e"]["d"] + pics["e"]["s"] + " " + pics["e"]["se"],

        // -----

        pics["sw"]["nw"] + " " + pics["sw"]["n"] + pics["sw"]["u"] + pics["sw"]["ne"] +
        pics["s"]["nw"]  + " " + pics["s"]["n"]  + pics["s"]["u"]  + pics["s"]["ne"]  +
        pics["se"]["nw"] + " " + pics["se"]["n"] + pics["se"]["u"] + pics["se"]["ne"],

        pics["sw"]["w"] + (roomOb["sw"] ? roomOb["sw"]->query_room_map_bracket() : "   ") + pics["sw"]["e"] +
        pics["s"]["w"]  + (roomOb["s"]  ? roomOb["s"]->query_room_map_bracket() : "   ") + pics["s"]["e"]  +
        pics["se"]["w"] + (roomOb["se"] ? roomOb["se"]->query_room_map_bracket() : "   ") + pics["se"]["e"],

        pics["sw"]["sw"] + pics["sw"]["d"] + pics["sw"]["s"] + " " + pics["sw"]["se"] +
        pics["s"]["sw"]  + pics["s"]["d"]  + pics["s"]["s"]  + " "  + pics["s"]["se"]  +
        pics["se"]["sw"] + pics["se"]["d"] + pics["se"]["s"] + " " + pics["se"]["se"],
    });
}

/* ----- environment damage ----- */

void handle_environment_damage (object living) {
    int water;
    if (water = query_property("water") && water > 1) {
        switch (random(water + 2)) {
            case 0:
                message("action", "You struggle against the deep water.", living);
                message("action", living->query_cap_name() + " struggles against the deep water.", this_object(), living);

                living->handle_damage(living->query_max_hp() * 5 / 100, 0);
                break;

            default:
                break;
        }
    }
}