#include "human.h"

inherit STD_ROOM;
inherit M_STORY;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. An open grassy area is to the north. A framework of hexagons glimmer overhead.");
    set_exits(([
        "north": HUMAN_ROOM + "park_southeast.c",
        "south": HUMAN_ROOM + "spruce_st_s5.c",
    ]));
    set_looks(([
        "street": "A village street labeled S Spruce St.",
    ]));
    set_story_lines(({
        "You get the feeling that you are being watched.",
    }));
    set_reset(([
        HUMAN_NPC + "teen_suspicious.c": 1,
    ]));
}

int handle_receive (object ob) {
    int result = ::handle_receive(ob);
    if (result && ob->is_character()) {
        story_start(ob);
    }
    return result;
}