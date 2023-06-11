#include "human.h"

inherit STD_ROOM;

nosave private mapping syntax = ([
    "encode": format_syntax("encode [attribute] [value]"),
    "encode_gender": format_syntax("encode gender female|male|neither"),
    "encode_build": format_syntax("encode build athletic|lean|muscular|stout|slender"),
    "encode_complexion": format_syntax("encode complexion fair|medium|dark"),
    "encode_eye": format_syntax("encode eye amber|black|blue|brown|green|hazel"),
    "encode_hair": format_syntax("encode hair auburn|bald|black|blonde|brown|gray"),
    "encode_height": format_syntax("encode height 160-180"),

    "randomize": format_syntax("randomize"),
    "done": format_syntax("done"),
    "look": format_syntax("look"),
    "download": format_syntax("download [class]"),
    "download full": format_syntax("download warrior|mystic|scoundrel|ranger|psionist|paladin"),
]);

private void randomize_attributes (object tc) {
    mapping attributes = D_SPECIES->query_species()["human"]["attributes"];

    tc->set_attribute("build", element_of(attributes["build"]));
    tc->set_attribute("complexion", element_of(attributes["complexion"]));
    tc->set_attribute("eye", element_of(attributes["eye"]));
    tc->set_attribute("hair", element_of(attributes["hair"]));
    tc->set_attribute("height", "" + (attributes["height"]["min"] + random(attributes["height"]["max"] - attributes["height"]["min"] + 1)));
}

string prepare_long_footer () {
    object tc = this_character();
    string fmt = "%-14s %-20s";
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
                "header": ({ "Attributes", "Stats" }),
                "items": ({
                    sprintf(fmt, "CLASS:", tc->query_class()),
                    sprintf(fmt, "PERCEPTION:", ""+tc->query_stat("perception")),
                    sprintf(fmt, "GENDER:", tc->query_gender()),
                    sprintf(fmt, "STRENGTH:", ""+tc->query_stat("strength")),
                    sprintf(fmt, "BUILD:", ""+tc->query_attribute("build")),
                    sprintf(fmt, "ENDURANCE:", ""+tc->query_stat("endurance")),
                    sprintf(fmt, "COMPLEXION:", ""+tc->query_attribute("complexion")),
                    sprintf(fmt, "CHARISMA:", ""+tc->query_stat("charisma")),
                    sprintf(fmt, "EYE:", ""+tc->query_attribute("eye")),
                    sprintf(fmt, "INTELLIGENCE:", ""+tc->query_stat("intelligence")),
                    sprintf(fmt, "HAIR:", ""+tc->query_attribute("hair")),
                    sprintf(fmt, "AGILITY:", ""+tc->query_stat("agility")),
                    sprintf(fmt, "HEIGHT:", ""+tc->query_attribute("height")),
                    sprintf(fmt, "LUCK:", ""+tc->query_stat("luck")),
                }),
                "columns": 2,
                "align": "left",
            ]),
        }),
        "footer": ([
            "header": ({ "Actions" }),
            "items": ({
                syntax["download"],
                syntax["encode"],
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

int handle_receive (object ob) {
    if (ob && ob->is_character()) {
        if (ob->query_species() == "human" && !sizeof(ob->query_attributes())) {
            randomize_attributes(ob);
        }
    }
    return ::handle_receive(ob);
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


    if (member_array(word, ({ "build", "complexion", "eye", "gender", "hair", "height"})) == -1) {
        write("Syntax: "+syntax["encode"]+"\n");
        return;
    }

    if (!sizeof(str) && member_array(word, ({ "build", "complexion", "eye", "gender", "hair", "height"})) > -1) {
        write("Syntax: "+syntax["encode_"+word]+"\n");
        return;
    }

    if (word == "gender") {
        if (member_array(str, ({ "female", "male", "neither" })) == -1) {
            write("Syntax: "+syntax["encode_gender"]+"\n");
            return;
        }

        if (str == tc->query_gender()) {
            write("You are already " + str + ".\n");
            return;
        }

        tc->set_gender(str);
        write("You tap the " + str + " button on the display.\n");
        write("A shock arcs through your body!\n");
        write("You encode yourself " + str + ".\n");
    } else if (word == "build") {
        if (member_array(str, ({ "athletic", "lean", "muscular", "slender", "stout", })) == -1) {
            write("Syntax: "+syntax["encode_build"]+"\n");
            return;
        }

        if (str == tc->query_attribute("build")) {
            write("You already have a " + str + " build.\n");
            return;
        }

        tc->set_attribute("build", str);
        write("You tap the " + str + " build button on the display.\n");
        write("A shock arcs through your body!\n");
        write("You encode yourself with a " + str + " build.\n");
    } else if (word == "complexion") {
        if (member_array(str, ({ "fair", "medium", "dark", })) == -1) {
            write("Syntax: "+syntax["encode_complexion"]+"\n");
            return;
        }

        if (str == tc->query_attribute("complexion")) {
            write("You already have a " + str + " complexion.\n");
            return;
        }

        tc->set_attribute("complexion", str);
        write("You tap the " + str + " complexion button on the display.\n");
        write("A shock arcs through your body!\n");
        write("You encode yourself with a " + str + " complexion.\n");
    } else if (word == "eye") {
        if (member_array(str, ({ "amber", "black", "blue", "brown", "green", "hazel", })) == -1) {
            write("Syntax: "+syntax["encode_eye"]+"\n");
            return;
        }

        if (str == tc->query_attribute("eye")) {
            write("You are already " + str + " eyed.\n");
            return;
        }

        tc->set_attribute("eye", str);
        write("You tap the " + str + " eye button on the display.\n");
        write("A shock arcs through your body!\n");
        write("You encode yourself with " + str + " eyes.\n");
    } else if (word == "hair") {
        if (member_array(str, ({ "auburn", "bald", "black", "blonde", "brown", "gray", })) == -1) {
            write("Syntax: "+syntax["encode_hair"]+"\n");
            return;
        }

        if (str == tc->query_attribute("hair")) {
            write("You already have " + str + " hair.\n");
            return;
        }

        tc->set_attribute("hair", str);
        write("You tap the " + str + " hair button on the display.\n");
        write("A shock arcs through your body!\n");
        write("You encode yourself with " + str + " hair.\n");
    } else if (word == "height") {
        int height = to_int(str); // (attributes["height"]["min"] + random(attributes["height"]["max"] - attributes["height"]["min"] + 1))

        if (height < 160 || height > 180) {
            write("Syntax: "+syntax["encode_height"]+"\n");
            return;
        }

        if (str == tc->query_attribute("height")) {
            write("You already are " + str + " tall.\n");
            return;
        }
        tc->set_attribute("height", str);
        write("You tap the height slider at " + str + " on the display.\n");
        write("A shock arcs through your body!\n");
        write("You encode yourself with " + str + " height.\n");
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

    randomize_attributes(tc);

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