#include "human.h"

inherit STD_ROOM;

nosave private mapping syntax = ([
    "become": format_syntax("female|male|neither"),
    "randomize": format_syntax("randomize"),
    "done": format_syntax("done"),
    "look": format_syntax("look"),
    "retry": format_syntax("retry"),
]);

string prepare_long_footer () {
    object tc = this_character();
    border(([
        "title": "Specimen Status",
        "header": ([
            "items": ({
                "%^RED%^BOLD%^ERROR-" + (1001+random(9000))+"-"+(10001+random(90000)) + "%^RESET%^",
                "Automatic knowledge transfer unable to proceed.",
                "Manual selection mode.",
            }),
            "columns": 1,
            "align": "left",
        ]),
        "body": ([
            "items": ({
                sprintf("%10s %-20s", "Species:", tc->query_species()),
                sprintf("%10s %-20i", "Age:", 18),
                sprintf("%10s %-20s", "Gender:", tc->query_gender()),
            }),
            "columns": 1,
            "align": "center",
        ]),
        "footer": ([
            "header": ({ "Options" }),
            "items": ({
                syntax["become"],
                syntax["randomize"],
                "",
                "",
                syntax["done"],
            }),
            "columns": 2,
        ]),
        "borderColors": ({ ({ 65, 105, 225 }), ({ 65, 105, 225 }) }),
    ]));
}

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]));
    set_short("inside a tank");
    set_long("The embrace of warm air in the confinement of a transparent cylinder.\n\nA diagnostic display is projected onto the glass. There is a blinking red section displayed with an error code.");
    set_long_footer((: prepare_long_footer :));
    parse_init();
    parse_add_rule("female", "");
    parse_add_rule("male", "");
    parse_add_rule("neither", "");
    parse_add_rule("randomize", "");
    parse_add_rule("done", "");
}


/* ----- parser rules: female, male, neither ----- */

mixed can_female () {
    return environment(this_character()) == this_object();
}
void do_female () {
    object tc = this_character();

    if (tc->query_gender() == "female") {
        message("action", "You are already female.", tc);
        return;
    }

    tc->set_gender("female");
    message("action", "You tap the button on the display.", tc);
    message("action", "A shock arcs through your body!", tc);
}
mixed can_male () {
    return environment(this_character()) == this_object();
}
void do_male () {
    object tc = this_character();

    if (tc->query_gender() == "male") {
        message("action", "You are already male.", tc);
        return;
    }

    tc->set_gender("male");
    message("action", "You tap the button on the display.", tc);
    message("action", "A shock arcs through your body!", tc);
}
mixed can_neither () {
    return environment(this_character()) == this_object();
}
void do_neither () {
    object tc = this_character();

    if (tc->query_gender() == "neither") {
        message("action", "You are already neither gender.", tc);
        return;
    }

    tc->set_gender("neither");
    message("action", "You tap the button on the display.", tc);
    message("action", "A shock arcs through your body!", tc);
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

    tc->set_gender(gender = ({ "female", "male", "neither" })[random(3)]);
}

/* ----- parser rule: done ----- */

mixed can_done () {
    return environment(this_character()) == this_object();
}
void do_done () {
    object tc = this_character();
    string err = "%^RED%^BOLD%^ERR-"+(1001+random(9000))+"-"+(1+random(9))+"%^RESET%^";

    message("action", "You press the done button and the tank glass pops open.", tc);
    message("say", "A robotic voice chimes: Warning, automatic knowledge transfer was not completed, specimen may ... " + err + "! please ret-retry-y...", environment(tc));
    tc->handle_go(HUMAN_ROOM + "tank_hallway" + (1 + random(3)), "eject", "tank");
    tc->describe_environment();
}