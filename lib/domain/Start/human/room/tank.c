#include "human.h"

inherit STD_ROOM;

nosave private mapping syntax = ([
    "female|male|neither": format_syntax("female|male|neither"),
    "randomize": format_syntax("randomize"),
    "done": format_syntax("done"),
    "look": format_syntax("look"),
    "download": format_syntax("download [class]"),
    "download full": format_syntax("download warrior|mystic|scoundrel|ranger|psionist|paladin"),
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
                sprintf("%10s %-20i", "AGE:", 18), // @TODO
                sprintf("%10s %-20s", "SPECIES:", tc->query_species()),
                sprintf("%10s %-20s", "GENDER:", tc->query_gender()),
                sprintf("%10s %-20s", "CLASS:", tc->query_class()),
            }),
            "columns": 1,
            "align": "center",
        ]),
        "footer": ([
            "header": ({ "Options" }),
            "items": ({
                syntax["female|male|neither"],
                syntax["download"],
                "",
                "",
                syntax["randomize"],
                syntax["done"],
            }),
            "columns": 2,
        ]),
        "borderColors": ({ ({ 65, 105, 225 }), ({ 65, 105, 225 }) }), // Royal Blue
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
    parse_add_rule("download", "");
    parse_add_rule("download", "STR");
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
    message("system", prepare_long_footer(), tc);
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
    message("system", prepare_long_footer(), tc);
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
    message("system", prepare_long_footer(), tc);
}

/* ----- parser rule: download ----- */
mixed can_download () {
    return environment(this_character()) == this_object();
}
void do_download () {
    message("action", "Syntax: " + syntax["download full"], this_character());
}
mixed can_download_str (mixed args...) {
    return environment(this_character()) == this_object();
}
void do_download_str (mixed args...) {
    object tc = this_character();
    string str;

    if (sizeof(args)) {
        str = args[0];
    }

    if (member_array(str, ({ "warrior", "mystic", "scoundrel", "ranger", "psionist", "paladin", })) == -1) {
        return do_download();
    }

    tc->set_class(str);
    message("action", "You tap the button on the display.", tc);
    message("action", "A shock arcs through your body!", tc);
    message("system", prepare_long_footer(), tc);
}

/* ----- parser rule: randomize ----- */

mixed can_randomize () {
    return environment(this_character()) == this_object();
}
void do_randomize () {
    object tc = this_character();

    tc->set_gender(element_of(({ "female", "male", "neither" })));
    tc->set_class(element_of(({ "warrior", "mystic", "scoundrel", "ranger", "psionist", "paladin", })));
    message("action", "You tap the button on the display.", tc);
    message("action", "A shock arcs through your body!", tc);
    message("system", prepare_long_footer(), tc);
}

/* ----- parser rule: done ----- */

mixed can_done () {
    return environment(this_character()) == this_object();
}
void do_done () {
    object tc = this_character();
    string err = "%^RED%^BOLD%^ERR-"+(1001+random(9000))+"-"+(1+random(9))+"%^RESET%^";

    if (tc->query_class() == "adventurer") {
        message("action", "You press the done button and nothing happens.", tc);
        message("say", "A robotic voice chimes: Specimen must have a class before process is complete.", environment(tc));
        return;
    }

    message("action", "You press the done button and the tank glass pops open.", tc);
    message("say", "A robotic voice chimes: Warning, automatic knowledge transfer was not completed, specimen may ... " + err + "! please ret-retry-y...", environment(tc));
    tc->handle_go(HUMAN_ROOM + "tank_hallway" + (1 + random(3)), "eject", "tank");
    tc->describe_environment();
}