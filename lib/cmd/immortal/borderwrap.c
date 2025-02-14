// This command currently doesn't work due to a bug in fluffos efun::terminal_color
void command (string input, mapping flags) {
    mapping b = query_border_charset();
    string left = implode(format_border(([
        "title": "LEFT",
        "body": ([
            "items": ({
                "Test. %^CYAN%^<test1>%^RESET%^" + format_syntax("<test2>"),
            }),
            "align": "left",
            "columns": 1,
        ]),
    ]), b, 40, 0), "\n");
    string right = implode(format_border(([
        "title": "RIGHT",
    ]), b, 40, 0), "\n");
    string *leftLines, *rightLines;
    string line, result = "";
    int l;

    leftLines = explode(wrap_ansi(left, 40), "\n");
    rightLines = explode(wrap_ansi(right, 40), "\n");

    l = max(({ sizeof(leftLines), sizeof(rightLines) }));
    for (int i = 0; i < l; i ++) {
        line = "";
        if (i < sizeof(leftLines)) {
            line += leftLines[i];
        } else {
            line += sprintf("%*' 's", 40, "");
        }
        if (i < sizeof(rightLines)) {
            line += rightLines[i];
        }
        result += line + "\n";
    }
    message("system", result, this_character());
}