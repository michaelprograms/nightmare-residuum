#include "human.h"

inherit STD_ROOM;
inherit M_STORY;

void story_action_final (object character) {
    set_exit("out north", HUMAN_ROOM + "obelisk_structure.c", 0, function (object ob, string dir) {
        this_object()->remove_exit(dir);
        message("action", "The doors close behind you.\n", ob);
        message("action", "The doors on the southern wall close behind " + ob->query_cap_name() + ".\n", environment(ob), ob);
    });
}

void create () {
    ::create();
    set_short("a small room");
    set_long("An enclosed room with smooth blank walls.");
    set_story_lines( ({
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
        message("action", "\nThe doors close behind you.\n\n", ob);
    }
    return result;
}