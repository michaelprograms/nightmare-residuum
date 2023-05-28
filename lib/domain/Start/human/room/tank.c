#include "human.h"

inherit STD_ROOM;

nosave private mapping syntax = ([
    "become": format_syntax("become female|male|neither"),
    "randomize": format_syntax("randomize"),
    "done": format_syntax("done"),
    "look": format_syntax("look"),
    "retry": format_syntax("retry"),
]);

string prepare_long () {
    string long = "The embrace of warm air in the confinement of a transparent cylinder.";
    object tc = this_character();
    long += "\n\nA diagnostic display is projected onto the glass. There is a blinking red section displayed with an error code.\n\n";
    long += "%^BOLD%^Specimen Status%^RESET%^\n";
    long += sprintf("%-16s", "Species:") + " " + tc->query_species() + "\n";
    long += sprintf("%-16s", "Age:") + " " + 18 + "\n"; // @TODO
    long += sprintf("%-16s", "Gender:") + " " + tc->query_gender() + "\n";
    long += sprintf("%-16s", "Status:") + " Healthy\n\n";
    long += "WARNING! %^RED%^BOLD%^ERR-" + (1001+random(9000))+"-"+(10001+random(90000)) + "%^RESET%^: knowledge assimilation unable to proceed.\nPress " + syntax["retry"] + " to restart procedure.\n\n";
    long += "Options:\n";
    long += "  " + syntax["become"] + "\n";
    long += "  " + syntax["randomize"] + "\n\n";
    long += "  " + syntax["done"] + "";
    return long;
}

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]));
    set_short("inside a tank");
    set_long((: prepare_long :));
    parse_init();
    parse_add_rule("retry", "");
    parse_add_rule("become", "");
    parse_add_rule("become", "STR");
    parse_add_rule("randomize", "");
    parse_add_rule("done", "");
}

/* ----- parser rule: retry ----- */

mixed can_retry () {
    return environment(this_character()) == this_object();
}
void do_retry () {
    string err = "%^RED%^BOLD%^ERR-"+(11+random(90))+"-"+(101+random(900))+"%^RESET%^";

    write("You tap the retry button on the display.\n");
    write("\n%^RED%^BOLD%^BEEP BEEP BEEP.%^RESET%^\n\n");
    message("say", "A robotic voice chimes: Warning, knowledge assimilation cannot be initialized on this specimen. Initialize sterilization procedure...\n\n" + err + "! aborting process...", environment(previous_object()));
}

/* ----- parser rule: become ----- */

mixed can_become () {
    return environment(this_character()) == this_object();
}
void do_become () {
    write("Syntax: " + syntax["become"] + "\n");
}
mixed can_become_str (mixed args...) {
    return environment(this_character()) == this_object();
}
void do_become_str (mixed args...) {
    object tc = this_character();
    string str;

    if (sizeof(args)) {
        str = args[0];
    }

    if (member_array(str, ({ "female", "male", "neither" })) == -1) {
        return do_become();
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

/* ----- parser rule: randomize ----- */

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

/* ----- parser rule: done ----- */

mixed can_done () {
    return environment(this_character()) == this_object();
}
void do_done () {
    string err = "%^RED%^BOLD%^ERR-"+(1001+random(9000))+"-"+(1+random(9))+"%^RESET%^";

    message("action", "You press the done button and the tank glass pops open.", this_character());
    message("say", "A robotic voice chimes: Warning, knowledge assimilation process was not completed, specimen may ... " + err + "! please retry...", environment(previous_object()));
    this_character()->handle_go(HUMAN_ROOM + "tank_hallway" + (1 + random(3)), "eject", "tank");
    this_character()->describe_environment();
}