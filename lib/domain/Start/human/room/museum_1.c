#include <human.h>

inherit STD_ROOM;

int admit_entrance (object ob, string dir) {
    object npc;
    if (ob->is_character()) {
        if (present("museum ticket", ob)) {
            return 1;
        } else if (npc = present("museum attendant", environment(ob))) {
            npc->handle_command("say You need to buy a ticket before you are admitted entrance.");
            return 0;
        }
    }
    return 1;
}

void create () {
    ::create();
    set_properties(([ "indoors": 1, "no attack": 1, ]));
    set_short("a spacious lobby");
    set_long("The entrance to a building consisting of a spacious lobby with high ceilings and polished floors. The walls are made of stone blocks and covered with tapestries. A grand staircase along the back of the lobby leads up towards another floor that looks down upon the lobby. Hallways branch to the north and west, heading to the wings of the building.");
    set_exits(([
        "out southeast": HUMAN_ROOM "park_nw.c",
    ]));
    set_exit("north", HUMAN_ROOM + "museum_1_n1.c", (: admit_entrance :));
    set_exit("west", HUMAN_ROOM + "museum_1_w1.c", (: admit_entrance :));
    set_exit("up", HUMAN_ROOM + "museum_2.c", (: admit_entrance :));
    set_looks(([
        ({ "building", "ceiling", "walls" }): "A building of large proportions. The lobby is a large room with high ceilings and distant walls.",
        "lobby": "The entrance to the building, it features several different paths to traverse through.",
        "floor": "The floor is a polished white stone in large blocks.",
        "staircase": "The staircase is in the back of the lobby. The steps are wide stones with a short wall as a railing.",
        ({ "hallways", "wings" }): "Two hallways branch off from the main lobby to the north and west, leading to the wings of the building.",
    ]));
    set_reset(([
        HUMAN_NPC + "museum_attendant_entry.c": 1,
    ]));
}