#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, "no attack": 1, ]));
    set_short("a balcony over a lobby");
    set_long("Above the entrance to a building consisting of a spacious lobby. The ceilings are high above the polished floors below. The walls are made of stone blocks and covered with tapestries. The walls are made of stone blocks and covered with tapestries. A grand staircase leads down towards the lobby below. Hallways branch to the north and west, heading to the wings of the building.");
    set_exits(([
        "north": HUMAN_ROOM + "museum_2_n1.c",
        "west": HUMAN_ROOM + "museum_2_w1.c",
    ]));
    set_exit("down", HUMAN_ROOM + "museum_1.c", function(object po, string dir) {
        object ob = present("synthetic creature");
        if (po->is_character() && !po->query_achievement("human_museum")) {
            ob->handle_command("say " + po->query_cap_name() + ", one as young as yourself should really look through the rest of this museum. You are nearly done.");
            return 0;
        }
        return 1;
    });
    set_looks(([
        ({ "building", "ceiling", "walls" }): "A building of large proportions. The lobby is a large room with high ceilings and distant walls.",
        "floor": "The floor is a polished white stone in large blocks.",
        "staircase": "The staircase is in the back of the lobby. The steps are wide stones with a short wall as a railing.",
        ({ "hallways", "wings" }): "Two hallways branch off from the main lobby to the north and west, leading to the wings of the building.",
    ]));
    set_reset(([
        HUMAN_NPC + "museum_custodian.c": 1,
    ]));
}