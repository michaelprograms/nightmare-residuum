inherit STD_ITEM;
inherit M_LEVELABLE;

mapping __Resource = ([
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
]);

void create () {
    ::create();
    set_name("resource node");
    set_id(({"resource", "resource node", "node", "resource_node", }));
    set_short("a resource node");
    set_long("A resource node.");
    set_no_get(1);
}

void set_type (string type) {
    set_short("a rock containing " + type + " ore");
    set_long("A rock containing a strip of " + type + " ore.");
    set_id(({ type + " ore", type }));
}

void set_level (int l) {
    levelable::set_level(l);

    if (__Resource[l]) {
        set_type(__Resource[l]);
    }
}