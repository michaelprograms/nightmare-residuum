#include "human.h"

inherit STD_ROOM;

nosave private mapping syntax = ([
    "encode": format_syntax("encode gender|hair|eye|skin|body [attribute]"),
    "encode_gender": format_syntax("encode gender female|male|neither"),
    "encode_body": format_syntax("encode body [build]"),
    "encode_eye": format_syntax("encode eye [color]"),
    "encode_hair": format_syntax("encode hair [color]"),
    "encode_height": format_syntax("encode height [num]"),
    "encode_skin": format_syntax("encode skin [color]"),

    "randomize": format_syntax("randomize"),
    "done": format_syntax("done"),
    "look": format_syntax("look"),
    "download": format_syntax("download [class]"),
    "download full": format_syntax("download warrior|mystic|scoundrel|ranger|psionist|paladin"),
]);

string prepare_long_footer () {
    object tc = this_character();
    string fmt = "%-10s %-20s";
    border(([
        "title": "STATUS",
        "subtitle": "%^RED%^BOLD%^ERROR-" + (1001+random(9000))+"-"+(10001+random(90000)) + "%^RESET%^",
        "header": ([
            "items": ({
                "Automatic knowledge transfer unable to proceed.",
                "Manual selection mode.",
            }),
            "columns": 1,
            "align": "left",
        ]),
        "body": ({
            ([
                "header": ({ "Select", "Stats" }),
                "items": ({
                    sprintf(fmt, "CLASS:", tc->query_class()),
                    sprintf(fmt, "PERCEPTION:", ""+tc->query_stat("perception")),
                    sprintf(fmt, "GENDER:", tc->query_gender()),
                    sprintf(fmt, "STRENGTH:", ""+tc->query_stat("strength")),
                    sprintf(fmt, "BUILD:", ""+tc->query_attribute("body")),
                    sprintf(fmt, "ENDURANCE:", ""+tc->query_stat("endurance")),
                    sprintf(fmt, "COMPLEXION:", ""+tc->query_attribute("skin")),
                    sprintf(fmt, "CHARISMA:", ""+tc->query_stat("charisma")),
                    sprintf(fmt, "HAIR:", ""+tc->query_attribute("hair")),
                    sprintf(fmt, "INTELLIGENCE:", ""+tc->query_stat("intelligence")),
                    sprintf(fmt, "HEIGHT:", ""+tc->query_attribute("height")),
                    sprintf(fmt, "AGILITY:", ""+tc->query_stat("agility")),
                    sprintf(fmt, "EYE:", ""+tc->query_attribute("eye")),
                    sprintf(fmt, "LUCK:", ""+tc->query_stat("luck")),
                }),
                "columns": 2,
                "align": "left",
            ]),
        }),
        "footer": ([
            "header": ({ "Actions" }),
            "items": ({
                syntax["encode_gender"],
                syntax["encode_body"],
                syntax["encode_eye"],
                syntax["encode_hair"],
                syntax["encode_height"],
                syntax["encode_skin"],
                "",
                syntax["download"],
                "",
                syntax["randomize"],
                syntax["done"],
            }),
            "columns": 1,
        ]),
        "borderColors": ({ ({ 65, 105, 225 }), ({ 65, 105, 225 }) }), // Royal Blue
    ]));
}

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]));
    set_short("inside a tank");
    set_long("The embrace of warm air in the confinement of a transparent cylinder.\n\nA diagnostic display is projected onto the glass.");
    set_long_footer((: prepare_long_footer :));
    parse_init();
    parse_add_rule("encode", "");
    parse_add_rule("encode", "STR");
    parse_add_rule("download", "");
    parse_add_rule("download", "STR");
    parse_add_rule("randomize", "");
    parse_add_rule("done", "");
}

/* ----- parser rule: encode ----- */

mixed can_encode () {
    return environment(this_character()) == this_object();
}
void do_encode () {
    write("Syntax: " + syntax["encode"] + "\n");
}
mixed can_encode_str (mixed args...) {
    return environment(this_character()) == this_object();
}
void do_encode_str (mixed args...) {
    object tc = this_character();
    string str, *words, word;

    if (sizeof(args)) {
        str = args[0];
    }

    words = explode(str, " ");

    if (!sizeof(words)) {
        return do_encode();
    }

    word = words[0];
    str = implode(words[1..], " ");

    if (word == "gender") {
        if (member_array(str, ({ "female", "male", "neither" })) == -1) {
            return do_encode();
        }

        if (str == tc->query_gender()) {
            write("You are already " + str + ".\n");
            return;
        }

        tc->set_gender(str);
        write("You tap the " + str + " button on the display.\n");
        write("A shock arcs through your body!\n");
        write("You encode yourself " + str + ".\n");
    } else {
        return do_encode();
    }
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