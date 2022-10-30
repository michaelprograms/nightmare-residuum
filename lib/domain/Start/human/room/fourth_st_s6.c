#include "human.h"

inherit STD_ROOM;
inherit M_STORY;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A wide run-down building with boarded windows but open doors sits on the east side of the street. A framework of hexagons glimmer overhead. A concrete wall at the base of the hexagon dome blocks further passage south.");
    set_listen("default", "Several whispers can be heard from some scoundrels hanging around.");
    set_smell("default", "The smell of a warm musty run-down building.");
    set_exits(([
        "north": HUMAN_ROOM + "fourth_st_s5.c",
        "west": HUMAN_ROOM + "pine_st_e2.c",
        "enter southeast": HUMAN_ROOM + "intro_scoundrel.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled S. Fourth St.",
        "building": "The building looks to be a scoundrel hall.",
    ]));
    set_story_lines(({
        "You get the feeling that you are being watched.",
    }));
    set_reset(([
        HUMAN_NPC + "teen_suspicious.c": 3,
    ]));
}

int handle_receive (object ob) {
    int result = ::handle_receive(ob);
    if (result && ob->is_character()) {
        story_start(ob);
    }
    return result;
}