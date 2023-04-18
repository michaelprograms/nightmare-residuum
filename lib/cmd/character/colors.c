inherit STD_COMMAND;

void create () {
    set_syntax("colors");
    set_help_text("The colors command is used to view the list of supported colors.");
}

void command (string input, mapping flags) {
    string *keys = D_ANSI->query_keys();

    string *styles = ({ "BOLD", "BOLD_OFF", "ITALIC", "ITALIC_OFF", "UNDERLINE", "UNDERLINE_OFF", "STRIKE", "STRIKE_OFF", "INVERSE", "INVERSE_OFF", "RESET" });
    string *basicColors = ({ "RED", "ORANGE", "YELLOW", "BLUE", "CYAN", "GREEN", "MAGENTA", "BLACK", "WHITE", " ", "B_RED", "B_ORANGE", "B_YELLOW", "B_BLUE", "B_CYAN", "B_GREEN", "B_MAGENTA", "B_BLACK", "B_WHITE", });
    string *xtermColors = keys - styles - basicColors;

    styles = map(styles, (: ($1 != " " ? "%^"+$1+"%^" : "")+sprintf("%13|s", $1)+" %^RESET%^" :));

    basicColors = map(basicColors, (: ($1 != " " ? "%^"+$1+"%^" : "")+($1 == "BLACK" ? "B_WHITE%^" : "")+($1 == "B_WHITE" ? "BLACK%^" : "")+sprintf("%13|s", $1)+" %^RESET%^" :));

    xtermColors = filter(xtermColors, (: !regexp($1, "^B_") :));
    xtermColors = sort_array(xtermColors, function(string a, string b) {
        if(regexp(a, "[0-9]") && !regexp(b, "[0-9]")) return 1;
        if(!regexp(a, "[0-9]") && regexp(b, "[0-9]")) return -1;
        return strcmp(a, b);
    });
    xtermColors = xtermColors[0..<25] + ({ " ", " ", " ", " ", " ", " ", " ", " " }) + xtermColors[<24..];
    xtermColors = map(xtermColors, (: ($1 != " " ? "%^"+$1+"%^" : "")+(member_array($1, ({"AAA", "G01","G02","G03","G04","C16"})) > -1 ? "B_WHITE%^" : "")+" "+sprintf("%3|s", $1)+" %^RESET%^" :));

    border(([
        "title": "COLORS",
        "body": ({
            ([
                "header": ({ "Styles" }),
                "items": styles,
                "columns": 5,
            ]),
            ([
                "header": ({ "Colors" }),
                "items": basicColors,
                "columns": 5,
            ]),
            ([
                "items": xtermColors,
                "columns": 12,
            ]),
        }),
    ]));
}