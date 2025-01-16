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

string query_type () {
    return __ResourceType;
}
void set_type (string type) {
    if (member_array(type, keys(__Resource)) == -1) {
        return;
    }
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
        set_name(resource + " " + __ResourceType);
    }
}

int direct_harvest_obj (mixed args...) {
    return environment() && environment() == environment(previous_object());
}

/**
 * Handle a harvest action made by a character to gather a resource.
 *
 * @param {STD_CHARACTER} character the source of the harvest action
 */
void handle_harvest (object character) {
    object ob, tool;
    string toolType, harvestableType;

    switch (__ResourceType) {
        case "ore":
            toolType = "pickaxe";
            break;
        case "wood":
            toolType = "axe";
            break;
        default:
            return;
    }

    tool = present(toolType, character);
    if (!tool) {
        message("action", "You need a " + toolType + " to harvest " + query_name() + ".", character);
        return;
    }

    message("action", "You harvest " + query_name() + ".", character);
    message("action", character->query_cap_name() + " harvests " + query_name() + ".", environment(character), character);

    ob = new("/std/resource/resource.c");
    ob->set_type(__ResourceType);

    harvestableType = __Resource[__ResourceType][query_level()];
    switch (__ResourceType) {
        case "ore":
            ob->set_short("a chunk of " + harvestableType + " ore");
            ob->set_long("A chunk containing a strip of " + harvestableType + " ore.");
            ob->set_id(({ "ore", "chunk", harvestableType + " ore", harvestableType }));
            ob->set_name(harvestableType + " ore");
            break;
        case "wood":
            ob->set_short("a log of " + harvestableType + " wood");
            ob->set_long("A log containing a strip of " + harvestableType + " wood.");
            ob->set_id(({ "wood", "log", harvestableType + " wood", harvestableType }));
            ob->set_name(harvestableType + " wood");
            break;
        default:
            return;
    }

    ob->handle_move(character);
    handle_remove();
}