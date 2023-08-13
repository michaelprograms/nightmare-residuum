#include "origin.h"

inherit STD_ROOM;
inherit M_STORY;

void story_action_final (object target) {
    message("action", "A gradually increasing brightness attracts your attention.", target);
    message("action", "You " + format_syntax("look") + " over your surroundings.", target);
    target->handle_move(ORIGIN_ROOM + "tank.c");
    target->describe_environment();
}

int handle_receive (object ob) {
    int result = ::handle_receive(ob);
    if (result && ob->is_character()) {
        story_start(ob);
    }
    return result;
}

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]));
    set_short("deep in darkness");
    set_long("");
    set_story_delay(4);
    set_story_lines(({
        "You are naught...nowhere...nobody...nothing...\n",
        "A shock of creation sparks existence into being.\n",
        "The pumping of blood pounds in your ears momentarily.\n",
        "Gasping breaths turn to rhythmic aspirations.\n",
        "All of your skin goosebumps at the sensation of flowing air.\n",
        "Your eyes finally shrug off the weight of unconsciousness.\n",
    }));
}