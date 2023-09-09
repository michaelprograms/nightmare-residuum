#include <planet.h>

inherit M_CLEAN;

int is_virtual_room () { return 1; }

object virtual_create (string arg) {
    string name, path;
    int x, y, xw, xe, yn, ys;
    object room;
    int pSize, pTerrain;
    mapping planet, *overrides;

    if (sscanf(arg, PLANET_V_ROOM + "terrain/%s/%d.%d", name, x, y) != 3) {
        return 0;
    }

    // prepare for room
    path = PLANET_V_ROOM + "terrain/" + name;
    planet = D_PLANET->query_planet(name);
    pSize = planet["size"] || 100;
    xw = x - 1 > -1     ? x - 1 : pSize - 1;
    xe = x + 1 < pSize  ? x + 1 : 0;
    yn = y - 1 >= 0     ? y - 1 : pSize - 1;
    ys = y + 1 < pSize  ? y + 1 : 0;

    // setup room
    room = new(PLANET_V_ROOM + "base/terrain.c");
    room->set_exit("northwest", path + "/" + xw + "." + yn + ".c");
    room->set_exit("north",     path + "/" + x  + "." + yn + ".c");
    room->set_exit("northeast", path + "/" + xe + "." + yn + ".c");
    room->set_exit("west",      path + "/" + xw + "." + y  + ".c");
    room->set_exit("east",      path + "/" + xe + "." + y  + ".c");
    room->set_exit("southwest", path + "/" + xw + "." + ys + ".c");
    room->set_exit("south",     path + "/" + x  + "." + ys + ".c");
    room->set_exit("southeast", path + "/" + xe + "." + ys + ".c");

    if (overrides = planet["overrides"]) {
        overrides = filter(overrides, (: $1["x"] == $(x) && $1["y"] == $(y) :));
        foreach (mapping override in overrides) {
            if (override["enter"]) {
                room->remove_exit(override["dir"]);
                room->set_exit("enter " + override["dir"], override["room"]);
            } else if (!override["room"]) {
                room->remove_exit(override["dir"]);
            } else {
                room->set_exit(override["dir"], override["room"]);
            }
        }
    }
    // setup terrain
    pTerrain = D_PLANET->query_planet_terrain(name, x, y, pSize);
    room->set_terrain(pTerrain);

    return room;
}