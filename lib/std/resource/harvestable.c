inherit STD_ITEM;
inherit M_LEVELABLE;

nosave private string *__NodeTypes = ({ "ore", "wood", "salvage" });

void create () {
    ::create();
    set_name("resource node");
    set_id(({ "resource_node", }));
    set_short("a resource node");
    set_long("A resource node.");
    set_no_get(1);
}

void set_type (string type) {
    if (member_array(type, __NodeTypes) == -1) {
        return;
    }
    item::set_type(type);
}

void set_level (int l) {
    string material, type;

    levelable::set_level(l);
    type = query_type();
    if (!sizeof(type)) {
        return;
    }
    material = STD_RESOURCE->query_material(type, query_level());
    if (!material) {
        return;
    }
    if (type == "ore") {
        set_short("a rock containing " + material + " ore");
        set_long("A rock containing a strip of " + material + " ore.");
        set_id(({ "ore", "rock", material + " ore", material }));
        set_name(material + " ore");
    } else if (type == "wood") {
        set_short("a log containing " + material + " wood");
        set_long("A log containing a strip of " + material + " wood.");
        set_id(({ "wood", "log", material + " wood", material }));
        set_name(material + " wood");
    } else if (type == "salvage") {
        set_short("a pile of debris containing salvaged " + material);
        set_long("A pile of debris containing salvageable " + material + ".");
        set_id(({ "salvage", "debris", material }));
        set_name(material + " salvage");
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
    string toolType, type = query_type();

    switch (type) {
        case "ore":
            toolType = "pickaxe";
            break;
        case "wood":
            toolType = "axe";
            break;
        case "salvage":
            toolType = "prybar";
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

    ob = new(STD_RESOURCE);
    ob->set_type(type);
    ob->set_level(query_level());
    ob->handle_move(character);
    handle_remove();
}
