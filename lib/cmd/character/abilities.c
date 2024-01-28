inherit STD_COMMAND;

#define ABILITY_DIR "/cmd/ability"

void create () {
    ::create();
    set_syntax("abilities (mine|npc|[class])");
    set_help_text("The abilities command is used to view the list of abilities, with filters for abilities your character can use, the generic NPC ability list, or the list of abilities for a class.");
}

void command (string input, mapping flags) {
    object po = previous_object();
    mixed *body = ({ });
    string *abilities = ({ });
    mapping b;
    string subtitle;
    string mode;

    if (sizeof(input)) {
        input = lower_case(input);
        if (input == "mine") {
            subtitle = "mine";
            mode = "mine";
        } else if (member_array(input, ({ "warrior", "mystic", "scoundrel", "ranger", "psionist", "paladin", "NPC" })) > -1) {
            subtitle = input;
            mode = "class";
        }
    }

    foreach (string action in D_COMMAND->query_abilities(ABILITY_DIR)) {
        if (mode == "mine") {
            object ability = load_object(ABILITY_DIR + "/" + action + ".c");
            if (ability->verify_ability_requirements(po)) {
                abilities += ({ action });
            }
        } else if (mode == "class") {
            object ability = load_object(ABILITY_DIR + "/" + action + ".c");
            mapping reqs = ability->query_ability_requirements();
            if (reqs[input]) {
                abilities += ({ action });
            }
        } else {
            // no input, show all abilities
            abilities += ({ action });
        }
    }

    b = ([
        "columns": 4,
        "items": ({ }),
    ]);
    foreach (string a in sort_array(abilities, 1)) {
        b["items"] += ({ a });
    }
    body += ({ b });

    border(([
        "title": "ABILITIES",
        "subtitle": subtitle,
        "body": body,
    ]));
}