inherit STD_ITEM;
inherit M_LEVELABLE;

mapping __Resource = ([
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
]);
string __ResourceType;

void create () {
    ::create();
    set_name("resource node");
    set_id(({ "resource_node", }));
    set_short("a resource node");
    set_long("A resource node.");
    set_no_get(1);
}

void set_type (string type) {
    __ResourceType = type;
}

void set_level (int l) {
    string resource;
    levelable::set_level(l);

    if (__ResourceType && (resource = __Resource[__ResourceType][l])) {
        if (__ResourceType == "ore") {
            set_short("a rock containing " + resource + " ore");
            set_long("A rock containing a strip of " + resource + " ore.");
            set_id(({ "ore", "rock", resource + " ore", resource }));
        } else if (__ResourceType == "wood") {
            set_short("a log containing " + resource + " wood");
            set_long("A log containing a strip of " + resource + " wood.");
            set_id(({ "wood", "log", resource + " wood", resource }));
        }
    }
}