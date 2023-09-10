inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("memory");
    set_help_text("The memory command is used to view the total memory used by the driver.");
}

void command (string input, mapping flags) {
    border(([
        "title": "MEMORY",
        "subtitle": mud_name(),
        "body": ([
            "header": ({ "Total Memory Usage", }),
            "items": ({
                (memory_info() / 1024.0 / 1024.0) + " mb"
            }),
            "columns": 1,
            "align": "left",
        ]),
    ]));
}