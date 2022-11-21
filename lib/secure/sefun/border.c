private mixed *query_character_border_colors () {
    object po = SEFUN->this_character();
    int *arr1, *arr2 = ({ 192, 192, 192 });

    switch (po->query_class()) {
        case "warrior":
            arr1 = ({ 220, 20, 60 });   // Crimson
            break;
        case "templar":
            arr1 = ({ 255, 215, 0 });   // Gold
            break;
        case "scoundrel":
            arr1 = ({ 255, 99, 71 });  // Tomato
            break;
        case "ranger":
            arr1 = ({ 34, 139, 34 });   // Forest Green
            break;
        case "psionist":
            arr1 = ({ 65, 105, 225 });  // Royal Blue
            break;
        case "paladin":
            arr1 = ({ 148, 0, 211 });   // Dark Violet
            break;
        default:
            arr1 = ({ 191, 63, 191 });
            arr2 = ({ 63, 191, 191 });
            break;
    }

    return ({ arr1, arr2 });
}

mapping query_border_charset () {
    string type;
    if (SEFUN->query_account_setting("screenreader") == "on") {
        type = "screenreader";
    } else {
        type = query_encoding();
    }
    return ([
        "utf-8": ([
            // lines
            "h": "─",
            "v": "│",
            // diagonals
            "dd": "╲",
            "du": "╱",
            // corners
            "tl": "┌",
            "tr": "┐",
            "br": "┘",
            "bl": "└",
            // joints
            "t": "┬",
            "b": "┴",
            "l": "├",
            "r": "┤",
            "x": "╳",
            "+": "┼",

        ]),
        "US-ASCII": ([
            // lines
            "h": "-",
            "v": "|",
            // diagonals
            "dd": "\\",
            "du": "/",
            // corners
            "tl": ".",
            "tr": ".",
            "br": "'",
            "bl": "'",
            // joints
            "t": "-",
            "b": "-",
            "l": "|",
            "r": "|",
            "x": "x",
            "+": "+",
        ]),
        "screenreader": ([
            // lines
            "h": " ",
            "v": " ",
            // diagonals
            "dd": " ",
            "du": " ",
            // corners
            "tl": " ",
            "tr": " ",
            "br": " ",
            "bl": " ",
            // joints
            "t": " ",
            "b": " ",
            "l": " ",
            "r": " ",
        ]),
    ])[type];
}

// create a unicode border around action output
// https://en.wikipedia.org/wiki/Box_Drawing
//
// mapping data = ([
// /*
//     "title": "TITLE",
//     "subtitle": "Subtitle",
//     "header": ([
//         "header": "string" || ({ /* array of strings */ })
//         "items": ({ }),
//         "columns": 2,
//         "align": "left|center",
//     ]),
//     "body": ([
//         "header": "string" || ({ /* array of strings */ }),
//         "items": ({ /* array of strings */ }),
//         "columns": 2,
//         "align": "left|center",
//     ]) || ({ /* array of mappings */ }),
//     "footer": ([
//         "header": "string" || ({ /* array of strings */ }),
//         "items": ({ /* array of strings */ }),
//         "columns": 2,
//         "align": "left|center",
//     ]),
// */
// ]);
string *format_border (mapping data) {
    string *lines = ({ }), line, *linesBody = ({ });

    int fTitle = !undefinedp(data["title"]) && data["title"];
    int fSubtitle = !undefinedp(data["subtitle"]) && data["subtitle"];
    int fHeader = !undefinedp(data["header"]) && data["header"];
    int fBody = !undefinedp(data["body"]) && data["body"];
    int fFooter = !undefinedp(data["footer"]) && data["footer"];

    mapping b; // border charset
    int ansi, width, n;
    string *colors, *colors2, *colorsBody, *colorsBody2;
    mixed *borderColors;
    string left, right;
    string format;

    if (fSubtitle) {
        data["subtitle"] = SEFUN->strip_colour(data["subtitle"]);
    }

    b = query_border_charset();
    ansi = SEFUN->query_account_setting("ansi") == "on";
    width = to_int(SEFUN->query_account_setting("width"));
    n = 0;

    if (ansi && SEFUN->query_account_setting("screenreader") != "on") {
        borderColors = query_character_border_colors();
        colors = SEFUN->color_gradient(borderColors[0], borderColors[1], width);
        colors2 = ({ });
        for (int i = sizeof(colors)-1; i >= 0; i --) {
            colors2 += ({ colors[i] });
        }
    } else {
        colors = allocate(width, "");
        colors2 = colors;
    }

    if (fTitle) {
        int titleLength = 0;

        // Title Line 1
        line = "   " + b["tl"] + sprintf("%'"+b["h"]+"'*s", 2 + strlen(data["title"]) + (fSubtitle ? 2 + strlen(data["subtitle"]) : 0), "") + b["tr"];
        if (ansi) {
            line = SEFUN->apply_gradient(line, colors);
        }
        lines += ({ line });

        // Title Line 2
        line = b["tl"] + (fHeader ? b["t"] : b["h"]) + b["h"];
        line += b["r"] + " " + data["title"];
        n += 5 + strlen(data["title"]);
        if (fSubtitle) {
            line += ": " + data["subtitle"];
            n += 2 + strlen(data["subtitle"]);
        }
        titleLength = n;
        line += " " + b["l"] + b["h"];
        n += 3;
        line += sprintf("%'"+b["h"]+"'*s", width-2-n, "");
        line += (fHeader ? b["t"] : b["h"]) + b["tr"];
        if (ansi) {
            line = SEFUN->apply_gradient(line[0..4], colors[0..4]) + "\e[0;37;40;1m" + replace_string(line[5..titleLength-1], ":", ":\e[22m") + SEFUN->apply_gradient(line[titleLength..], colors[titleLength..]);
        }
        lines += ({ line });

        // Title Line 3
        line = b["v"] + (fHeader ? b["v"] : " ") + " ";
        line += b["bl"] + sprintf("%'"+b["h"]+"'*s", 2 + strlen(data["title"]) + (data["subtitle"] ? 2 + strlen(data["subtitle"]) : 0), "") + b["br"];
        line += sprintf("%' '*s", width-1-n, "");
        line += (fHeader ? b["v"] : " ") + b["v"];
        if (ansi) {
            line = SEFUN->apply_gradient(line, colors);
        }
        lines += ({ line });

    } else { // no title
        line = b["tl"] + (fHeader ? b["t"] : b["h"]) + sprintf("%'"+b["h"]+"'*s", width-4, "") + (fHeader ? b["t"] : b["h"]) + b["tr"];
        if (ansi) {
            line = SEFUN->apply_gradient(line, colors);
        }
        lines += ({ line });
    }

    if (fHeader) {
        left = b["v"] + b["v"];
        right = b["v"] + b["v"];
        if (ansi) {
            left = SEFUN->apply_gradient(left, colors[0..1]);
            right = SEFUN->apply_gradient(right, colors[<2..<1]);
        }
        // Header header
        if (stringp(data["header"]["header"])) {
            line = left + "  " + (ansi?"\e[0;37;40;1m":"") + data["header"]["header"] + (ansi?"\e[22m":"") + sprintf("%' '*s", width-8-strlen(data["header"]["header"]), "") + "  " + right;
            lines += ({ line });
        } else if (arrayp(data["header"]["header"])) {
            if (!data["header"]["columns"]) {
                data["header"]["columns"] = 2;
            }
            if (sizeof(data["header"]["header"]) > data["header"]["columns"]) {
                data["header"]["header"] = data["header"]["header"][0..data["header"]["columns"]-1];
            }
            line = left + "  " + (ansi?"\e[0;37;40;1m":"") + format_page(data["header"]["header"], data["header"]["columns"], 4, (data["header"]["align"] == "center")) + (ansi?"\e[22m":"") + "  " + right;
            lines += ({ line });
        }
        // Header lines
        format = sizeof(data["header"]["items"]) > 0 ? format_page(data["header"]["items"], data["header"]["columns"], 4, (data["header"]["align"] == "center")) : "";
        foreach (string l in explode(format, "\n")) {
            line = left + "  " + l + "  " + right;
            lines += ({ line });
        }
        // Header bottom line
        line = b["v"] + b["bl"] + sprintf("%'"+b["h"]+"'*s", width-4, "") + b["br"] + b["v"];
        if (ansi) {
            line = SEFUN->apply_gradient(line, colors);
        }
        lines += ({ line });
    }

    if (fBody && mapp(data["body"])) {
        data["body"] = ({ data["body"] });
    }
    if (fBody && arrayp(data["body"])) {
        // Body top line
        line = sprintf("%' '*s", width-2, "");
        if (ansi) {
            line = SEFUN->apply_gradient(line, colors);
        }
        linesBody += ({ line });
        foreach (mapping child in data["body"]) {
            // Body child header
            if (stringp(child["header"])) {
                line = "   " + (ansi?"\e[0;37;40;1m":"") + child["header"] + (ansi?"\e[22m":"") + sprintf("%' '*s", width-8-strlen(child["header"]), "") + "   ";
                linesBody += ({ line });
            } else if (arrayp(child["header"])) {
                if (!child["columns"]) {
                    child["columns"] = 2;
                }
                if (sizeof(child["header"]) > child["columns"]) {
                    child["header"] = child["header"][0..child["columns"]-1];
                }
                line = "   " + (ansi?"\e[0;37;40;1m":"") + format_page(child["header"], child["columns"], 4, (child["align"] == "center")) + (ansi?"\e[22m":"") + "   ";
                linesBody += ({ line });
            }
            // Body child lines
            format = sizeof(child["items"]) > 0 ? format_page(child["items"], child["columns"], 4, (child["align"] == "center")) : "";
            foreach (string l in explode(format, "\n") - ({""})) {
                line = "   " + l + "   ";
                linesBody += ({ line });
            }
            // Body child bottom line
            line = sprintf("%' '*s", width-2, "");
            linesBody += ({ line });
        }
        if (ansi && SEFUN->query_account_setting("screenreader") != "on") {
            colorsBody = SEFUN->color_gradient(borderColors[0], borderColors[1], sizeof(linesBody) + 2);
            colorsBody = colorsBody[1..<2];
            colorsBody2 = ({ });
            for (int i = sizeof(colorsBody)-1; i >= 0; i --) {
                colorsBody2 += ({ colorsBody[i] });
            }
            for (int i = sizeof(linesBody)-1; i >= 0; i --) {
                left = SEFUN->apply_gradient(b["v"], ({ colorsBody[i] }));
                right = SEFUN->apply_gradient(b["v"], ({ colorsBody2[i] }));
                linesBody[i] = left + linesBody[i] + right;
            }
        } else {
            for (int i = sizeof(linesBody)-1; i >= 0; i --) {
                linesBody[i] = b["v"] + linesBody[i] + b["v"];
            }
        }

        lines += linesBody;
    }

    if (fFooter) {
        left = b["v"] + b["v"];
        right = b["v"] + b["v"];
        if (ansi) {
            left = SEFUN->apply_gradient(left, colors2[0..1]);
            right = SEFUN->apply_gradient(right, colors2[<2..<1]);
        }

        // Footer top line
        line = b["v"] + b["tl"] + sprintf("%'"+b["h"]+"'*s", width-4, "") + b["tr"] + b["v"];
        if (ansi) {
            line = SEFUN->apply_gradient(line, colors2);
        }
        lines += ({ line });
        // Footer header
        if (stringp(data["footer"]["header"])) {
            line = left + "  " + (ansi?"\e[0;37;40;1m":"") + data["footer"]["header"] + (ansi?"\e[22m":"") + sprintf("%' '*s", width-8-strlen(data["footer"]["header"]), "") + "  " + right;
            lines += ({ line });
        } else if (arrayp(data["footer"]["header"])) {
            if (!data["footer"]["columns"]) {
                data["footer"]["columns"] = 2;
            }
            if (sizeof(data["footer"]["header"]) > data["footer"]["columns"]) {
                data["footer"]["header"] = data["footer"]["header"][0..data["footer"]["columns"]-1];
            }
            line = left + "  " + (ansi?"\e[0;37;40;1m":"") + format_page(data["footer"]["header"], data["footer"]["columns"], 4, (data["footer"]["align"] == "center")) + (ansi?"\e[22m":"") + "  " + right;
            lines += ({ line });
        }
        // Footer lines
        format = sizeof(data["footer"]["items"]) > 0 ? format_page(data["footer"]["items"], data["footer"]["columns"], 4, (data["footer"]["align"] == "center")) : "";
        foreach (string l in explode(format, "\n")) {
            line = left + "  " + l + "  " + right;
            lines += ({ line });
        }
        // Border Bottom line
        line = b["bl"] + b["b"] + sprintf("%'"+b["h"]+"'*s", width-4, "") + b["b"] + b["br"];
        if (ansi) {
            line = SEFUN->apply_gradient(line, colors2);
        }
        lines += ({ line });
    } else {
        // Border Bottom line
        line = b["bl"] + sprintf("%'"+b["h"]+"'*s", width-2, "") + b["br"];
        if (ansi) {
            line = SEFUN->apply_gradient(line, colors2);
        }
        lines += ({ line });
    }

    return lines;
}

void border (mapping data) {
    SEFUN->this_user()->handle_pager(format_border(data));
}