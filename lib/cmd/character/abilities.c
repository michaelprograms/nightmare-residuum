inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("abilities");
    set_help_text("The abilities command is used to view the list of abilities your character can use.");
}

void command (string input, mapping flags) {
    string subtitle;
    mixed *body = ({ });
    string *abilities = ({ });
    mapping b;

    // @TODO handle cases for input == mine, input == [class]
    foreach (string action in D_COMMAND->query_abilities("/cmd/ability")) {
        abilities += ({ action });
    }

    b = ([
        "header": ({ "Abilities" }),
        "columns": 4,
        "items": ({ }),
    ]);
    foreach (string a in sort_array(abilities, 1)) {
        b["items"] += ({ a });
    }
    body += ({ b });

    border(([
        "title": "COMMANDS",
        "subtitle": subtitle,
        "body": body,
    ]));
}