#include "human.h"

inherit STD_ROOM;
inherit M_STORY;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north and south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "north": HUMAN_ROOM + "spruce_st_s4.c",
        "south": HUMAN_ROOM + "spruce_st_s6.c",
    ]));
    set_looks(([
        "street": "A village street labeled S Spruce St.",
    ]));
    set_story_lines(({
        "You get the feeling that you are being watched.",
    }));
    set_reset(([
        HUMAN_NPC + "teen_suspicious.c": 2,
    ]));
}

int handle_receive (object ob) {
    int result = ::handle_receive(ob);
    if (result && ob->is_character()) {
        story_start(ob);
    }
    return result;
}