inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("databases");
    set_help_text("The databases command is used to view the entire list of databases.");
}

void command (string input, mapping flags) {
    string *status = explode(db_status(), "\n") - ({ "" });
    if (!sizeof(status)) {
        status += ({ "No databases connected" });
    }
    border(([
        "title": "DATABASES",
        "body": ([
            "items": status,
            "columns": 1,
            "align": "center",
        ]),
    ]));
}