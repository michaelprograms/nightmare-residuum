#include <planet.h>

inherit M_CLEAN;

int is_virtual_room() { return 1; }

/* ----- called by master::compile_object ----- */

object virtual_create(string path) {
    string type, name;
    int x, y;
    mapping structure;
    object room;

    if (sscanf(
        path,
        PLANET_V_ROOM + "interior/%s/%s.%d.%d/entrance.c",
        type,
        name,
        x,
        y
    ) != 4) {
        return 0;
    }

    structure = D_PLANET->query_structure(name, x, y);
    if (!structure || structure["type"] != type) {
        return 0;  // no structure here, or a stale/forged path
    }

    room = new(PLANET_V_ROOM + "base/interior.c");
    room->set_property("type", type);
    room->set_property("level", structure["level"]);
    room->set_exit(
        "out",
        PLANET_V_ROOM + "surface/" + name + "/" + x + "." + y + ".c"
    );
    room->update_descriptions();
    room->update_contents();

    return room;
}
