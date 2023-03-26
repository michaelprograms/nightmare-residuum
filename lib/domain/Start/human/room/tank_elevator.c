#include "human.h"

inherit STD_ROOM;
inherit M_STORY;

void story_action_final (object character) {
    set_exit("out north", HUMAN_ROOM + "vault_structure.c", 0, function (object ob, string dir) {
        if (sizeof(this_object()->query_living_contents()) == 0) {
            this_object()->remove_exit(dir);
        }
        message("action", "The doors close behind you.", ob);
        message("action", "The doors on the southern wall close behind " + ob->query_cap_name() + ".", environment(ob), ob);
    });
}

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]))
    set_short("a small room");
    set_long("An enclosed room with smooth blank walls.");
    set_story_lines(({
        "The doors close behind you.",
        "You feel a downwards pull as the small room starts moving with upwards momentum.",
        "The pull begins to fade while air whistles past in a rush.",
        "The room shakes a little but continues gliding upwards.",
        "The upwards motion begins to slow and the whistling stalls out.",
        "With a ding, the doors open to the north, revealing a large cavernous area.",
    }));
}

int handle_receive (object ob) {
    int result = ::handle_receive(ob);
    if (result && ob->is_character()) {
        story_start(ob);
    }
    return result;
}