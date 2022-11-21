inherit STD_COMMAND;

void create () {
    set_syntax("callouts");
    set_help_text("The callouts command is used to view the entire list of callouts.");
}

void command (string input, mapping flags) {
    mixed *callouts;
    string *border, *items = ({ });
    int n;

    if (n = sizeof(callouts = call_out_info())) {
        foreach (mixed *callout in callouts) {
            callout[1] = replace_string(callout[1], "\n", "\\n");
            items += ({ callout... });
        }
    }

    border = format_border(([
        "title": "CALLOUTS",
        "subtitle": mud_name(),
        "body": ([
            "header": ({ "Object", "Function", "Delay", }),
            "items": items,
            "columns": 3,
        ]),
        "footer": ([
            "items": ({ n + " callout" + (n > 1 ? "s" : "") }),
            "columns": 1,
            "align": "center",
        ]),
    ]));
    foreach (string line in border) {
        message("system", line + "\n", this_character());
    }
}