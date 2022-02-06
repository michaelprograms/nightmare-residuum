inherit STD_ROOM;
inherit M_STORY;

string *query_story_lines (object target) {
    return ({
        "Doors quickly close behind you before seemlessly meeting the other walls.",
        "You feel a downwards pull as the room seems to be moving with an upwards momentum.",
        "The pull begins to subside with the whistling of air outside the enclosure.",
        "The room shakes a little but continues gliding upwards.",
        "The upwards motion begins to slow and the whistling stalls out.",
        "Suddenly the doors open before you, revealing a large enclosed area.",
    });
}

void close_door (object ob, string dir) {
    this_object()->remove_exit(dir);
}
void story_action_final (object target) {
    set_exit("north", "/domain/Start/human/structure.c", 0, (: close_door :));
}

void create () {
    ::create();
    set_short("a small room");
    set_long("An enclosed room with smooth blank walls.");
    set_exits(([

    ]));
}

int handle_receive (object ob) {
    int result = ::handle_receive(ob);
    if (result && ob->is_character()) {
        story_start(ob);
    }
    return result;
}