#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("a large stone structure");
    set_long("The interier cavity of a tall structure which growers narrower towards the top. Four stone walls enclose the chamber at a distance. Several lamps are scattered around the cavern, casting a white light across the room. Doorways can be seen on the northern and southern walls.");
    set_looks(([
        ({ "structure", "cavity" }): "Inside of a tall stone structure which is narrower at the top. Far above the top of the structure turns to darkness before a ceiling is seen.",
        "wall": "Dimly lit stone walls in the distance enclose the chamber.",
        "lamps": "The standing lamps brighten the center of the chamber substantially, but leave the walls dim in the distance.",
        "device": "It hangs weightlessly in the air, projecting a cone of %^CYAN%^BOLD%^light%^RESET%^ down upon the body.",
        "doorways": "There are identical doorways on the northern and southern walls, however there is a down arrow etched into the southern wall while the northern wall has an up arrow.",
    ]));
    set_exits(([
        "enter north": HUMAN_ROOM + "obelisk_elevator.c",
    ]));
    set_reset(([
        HUMAN_ITEM + "stasis_body.c": 1,
        HUMAN_NPC + "greeter.c": 1,
    ]));
    parse_init();
    parse_add_rule("done", "");
}

mixed can_done () {
    return environment(this_character()) == this_object();
}
void do_done () {
    message("action", "You stand upon the circle in the center of the chamber.\nEverything goes dark for a moment.\n", this_character());
    this_character()->handle_go(HUMAN_ROOM + "square.c", "teleport", "out of the obelisk", "out of the obelisk");
    this_character()->describe_environment();
}