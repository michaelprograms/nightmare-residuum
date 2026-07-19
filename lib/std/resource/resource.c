inherit STD_ITEM;
inherit M_LEVELABLE;

nosave private mapping __Resource = ([
    "ore": ([
        1: "aluminum",
        2: "tin",
        3: "copper",
        4: "iron",
        5: "lead",
        6: "silver",
        7: "tungsten",
        8: "gold",
        9: "platinum",
        10: "titanium",
    ]),
    "wood": ([
        1: "balsa",
        2: "fir",
        3: "cedar",
        4: "pine",
        5: "cypress",
        6: "oak",
        7: "birch",
        8: "maple",
        9: "ash",
        10: "teak",
    ]),
    "hide": ([
        1: "ragged",
        2: "patchy",
        3: "coarse",
        4: "mottled",
        5: "supple",
        6: "thick",
        7: "sleek",
        8: "hardened",
        9: "flawless",
        10: "primal",
    ]),
    "salvage": ([
        1: "wiring",
        2: "tubing",
        3: "sheeting",
        4: "plating",
        5: "fasteners",
        6: "casing",
        7: "circuitry",
        8: "servos",
        9: "capacitors",
        10: "alloy",
    ]),
]);

int is_resource() {
    return 1;
}

/* ----- tiers ----- */

private int calculate_tier(int level) {
    int tier;

    tier = (level + 1) / 2;
    if (tier < 1) {
        tier = 1;
    }
    if (tier > 10) {
        tier = 10;
    }
    return tier;
}

string *query_types() {
    return keys(__Resource);
}

string query_material(string type, int level) {
    if (!stringp(type) || !mapp(__Resource[type])) {
        return 0;
    }
    return __Resource[type][calculate_tier(level)];
}

/* ----- description ----- */

private void update_resource_description() {
    string type = query_type();
    string material;

    if (!sizeof(type)) {
        return;
    }
    material = query_material(type, query_level());
    if (!material) {
        return;
    }

    if (type == "ore") {
        set_short("a chunk of " + material + " ore");
        set_long("A chunk containing a strip of " + material + " ore.");
        set_id(({ "ore", "chunk", material + " ore", material }));
        set_name(material + " ore");
    } else if (type == "wood") {
        set_short("a log of " + material + " wood");
        set_long("A log containing a strip of " + material + " wood.");
        set_id(({ "wood", "log", material + " wood", material }));
        set_name(material + " wood");
    } else if (type == "hide") {
        set_short("a " + material + " hide");
        set_long("A " + material + " hide stripped from a creature.");
        set_id(({ "hide", material + " hide", material }));
        set_name(material + " hide");
    } else if (type == "salvage") {
        set_short("a piece of salvaged " + material);
        set_long("A piece of " + material + " salvaged from a structure.");
        set_id(({ "salvage", material, "salvaged " + material }));
        set_name("salvaged " + material);
    }
}

void set_type(string type) {
    if (!stringp(type) || member_array(type, keys(__Resource)) == -1) {
        error("Bad argument 1 to resource->set_type");
    }
    item::set_type(type);
    update_resource_description();
}

void set_level(int l) {
    levelable::set_level(l);
    update_resource_description();
}

void create() {
    ::create();
    set_name("resource");
    set_id(({ "resource", }));
    set_short("a resource");
    set_long("A resource.");
}
