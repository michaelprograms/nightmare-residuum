#include "human.h"

inherit STD_ROOM;
inherit M_STORY;

string *query_story_lines (object target) {
    return ({
        "You are naught...nowhere...nobody...nothing...",
        "A shock of creation sparks existence into being.",
        "The pumping of blood pounds in your ears momentarily.",
        "Gasping breaths turn to rhythmic aspirations.",
        "All of your skin goosebumps at the sensation of flowing air.",
        "Your eyes finally shrug off the weight of unconsciousness.",
    });
}

int handle_receive (object ob) {
    int result = ::handle_receive(ob);
    if (result && ob->is_character()) {
        story_start(ob);
    }
    return result;
}

string prepare_long () {
    object tc;
    string long = "The embrace of warm air in the confinement of a tank.";
    if (tc = this_character()) {
        long += "\n\nA diagnostic display is projected onto the glass.\n\n";
        long += sprintf("%-15s", "Species:") + " " + tc->query_species() + "\n";
        long += sprintf("%-15s", "Maturation Age:") + " " + 18 + "\n"; // @TODO
        long += sprintf("%-15s", "Gender:") + " " + tc->query_gender() + "\n";
        long += "Options:\n";
        long += "  %^CYAN%^[become female|male|neither]%^RESET%^\n";
        long += "  %^CYAN%^[randomize]%^RESET%^\n\n";
        long += "  %^CYAN%^[done]";
    }
    return long;
}

void create () {
    ::create();
    set_short("a tank");
    set_long((: prepare_long :));
    set_delay(4);
    set_no_clean(1);
    parse_init();
    parse_add_rule("become", "");
    parse_add_rule("become", "STR");
    parse_add_rule("randomize", "");
    parse_add_rule("done", "");
}

mixed can_become () {
    return environment(this_character()) == this_object();
}
void do_become () {
    write("Syntax: %^CYAN%^[become female|male|neither]%^RESET%^\n");
}
mixed can_become_str () {
    return environment(this_character()) == this_object();
}
void do_become_str (string str) {
    object tc = this_character();

    if (member_array(str, ({ "female", "male", "neither"})) == -1) {
        write("Syntax: %^CYAN%^[become female|male|neither]%^RESET%^\n");
        return;
    }

    if (str == tc->query_gender()) {
        write("You are already " + str + ".\n");
        return;
    }

    tc->set_gender(str);
    write("You tap the " + str + " button on the display.\n");
    write("A shock arcs through your body!\n");
    write("You become " + str + ".\n");
}

mixed can_randomize () {
    return environment(this_character()) == this_object();
}
void do_randomize () {
    object tc = this_character();
    string gender;

    write("You tap the randomize button on the display.\n");
    write("A shock arcs through your body!\n");

    tc->set_gender(gender = ({ "female", "male", "neither"})[random(3)]);
    write("You become " + gender + ".\n");
}

mixed can_done () {
    return environment(this_character()) == this_object();
}
void do_done () {
    message("action", "You press the done button.\nThe tank glass pops open.\n", this_character());
    this_character()->handle_go(HUMAN_ROOM + "tank_hallway" + (1 + random(3)), "eject", "out of the tank");
    this_character()->describe_environment();
    message("action", this_character()->query_name() + " is ejected out of the tank.\n", environment(this_character()), this_character());
}