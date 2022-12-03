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
//         "columns": 2 || ({ /* array of integer ratios */ }),
//         "align": "left|center",
//     ]),
//     "body": ([
//         "header": "string" || ({ /* array of strings */ }),
//         "items": ({ /* array of strings */ }),
//         "columns": 2 || ({ /* array of integer ratios */ }),
//         "align": "left|center",
//     ]) || ({ /* array of mappings */ }),
//     "footer": ([
//         "header": "string" || ({ /* array of strings */ }),
//         "items": ({ /* array of strings */ }),
//         "columns": 2 || ({ /* array of integer ratios */ }),
//         "align": "left|center",
//     ]),
// */
// ]);
string *format_border (mapping data) {
    string *lines = ({ }), line, *linesBody = ({ });
    mapping b; // border charset
    int width, n;
    string ansi, *colors, *colors2, *colorsBody, *colorsBody2;
    mixed *borderColors;
    string left, right, format;
    int columnWidth;

    int fTitle = !!(!undefinedp(data["title"]) && data["title"]);
    int fSubtitle = !undefinedp(data["subtitle"]) && sizeof(data["subtitle"]);
    int fHeader = !!(!undefinedp(data["header"]) && data["header"]);
    int fBody = !!(!undefinedp(data["body"]) && data["body"]);
    int fFooter = !!(!undefinedp(data["footer"]) && data["footer"]);

    b = query_border_charset();
    width = to_int(SEFUN->query_account_setting("width"));
    n = 0;
    if (SEFUN->query_account_setting("ansi") == "on") {
        ansi = this_user()->query_terminal_color();
    }

    if (ansi && SEFUN->query_account_setting("screenreader") != "on") {
        if (ansi == "256") {
            borderColors = query_character_border_colors();
            colors = SEFUN->color_gradient(borderColors[0], borderColors[1], width);
            colors2 = ({ });
            for (int i = sizeof(colors)-1; i >= 0; i --) {
                colors2 += ({ colors[i] });
            }
        }
    } else {
        colors = allocate(width, "");
        colors2 = colors;
    }

    if (fTitle) {
        int titleLength = 0;

        // Title Line 1
        line = "   " + b["tl"] + sprintf("%'"+b["h"]+"'*s", 2 + strlen(data["title"]) + (fSubtitle ? 2 + fSubtitle : 0), "") + b["tr"];
        if (ansi) {
            if (ansi == "256") {
                line = line[0..2] + SEFUN->apply_gradient(line[3..], colors);
            } else {
                line = "\e[36m" + line + "\e[0;37;40m";
            }
        }
        lines += ({ line });

        // Title Line 2
        line = b["tl"] + (fHeader ? b["t"] : b["h"]) + b["h"];
        line += b["r"] + " " + data["title"];
        n += 5 + strlen(data["title"]);
        if (fSubtitle) {
            data["subtitle"] = SEFUN->strip_colour(data["subtitle"]);
            line += ": " + data["subtitle"];
            n += 2 + fSubtitle;
        }
        titleLength = n;
        line += " " + b["l"] + b["h"];
        n += 3;
        line += sprintf("%'"+b["h"]+"'*s", width-2-n, "");
        line += (fHeader ? b["t"] : b["h"]) + b["tr"];
        if (ansi) {
            if (ansi == "256") {
                line = SEFUN->apply_gradient(line[0..3], colors[0..3]) + "\e[0;37;40;1m" + replace_string(line[4..titleLength], ":", ":\e[22m") + SEFUN->apply_gradient(line[titleLength+1..], colors[titleLength+1..]);
            } else {
                line = "\e[36m" + line[0..4] + "\e[0;37;40;1m" + line[5..titleLength-1] + "\e[22;36m" + line[titleLength..] + "\e[0;37;40m";
            }
        }
        lines += ({ line });

        // Title Line 3
        line = b["v"] + (fHeader ? b["v"] : " ") + " ";
        line += b["bl"] + sprintf("%'"+b["h"]+"'*s", 2 + strlen(data["title"]) + (data["subtitle"] ? 2 + fSubtitle : 0), "") + b["br"];
        line += sprintf("%*s", width-1-n, "");
        line += (fHeader ? b["v"] : " ") + b["v"];
        if (ansi) {
            if (ansi == "256") {
                line = SEFUN->apply_gradient(line[0..fHeader], colors[0..fHeader]) +
                        line[1+fHeader..2] +
                        SEFUN->apply_gradient(line[3..titleLength+1], colors[3..titleLength+1]) +
                        line[titleLength+2..<2+fHeader] +
                        SEFUN->apply_gradient(line[<1+fHeader..<1], colors[<1+fHeader..<1]);
            } else {
                line = "\e[36m" + line[0..fHeader] + "\e[0;37;40m" +
                line[1+fHeader..2] +
                "\e[36m" + line[3..titleLength+1] + "\e[0;37;40m" +
                line[titleLength+2..<2+fHeader] +
                "\e[36m" + line[<1+fHeader..<1] + "\e[0;37;40m";
            }
        }
        lines += ({ line });

    } else { // no title
        line = b["tl"] + (fHeader ? b["t"] : b["h"]) + sprintf("%'"+b["h"]+"'*s", width-4, "") + (fHeader ? b["t"] : b["h"]) + b["tr"];
        if (ansi) {
            if (ansi == "256") {
                line = SEFUN->apply_gradient(line, colors);
            } else {
                line = "\e[36m" + line + "\e[0;37;40m";
            }
        }
        lines += ({ line });
    }

    if (fHeader) {
        left = b["v"] + b["v"];
        right = b["v"] + b["v"];
        if (ansi) {
            if (ansi == "256") {
                left = SEFUN->apply_gradient(left, colors[0..1]);
                right = SEFUN->apply_gradient(right, colors[<2..<1]);
            } else {
                left = "\e[36m" + left + "\e[0;37;40m";
                right = "\e[36m" + right + "\e[0;37;40m";
            }
        }
        // Header header
        if (stringp(data["header"]["header"])) {
            line = left + "  " + (ansi?"\e[0;37;40;1m":"") + data["header"]["header"] + (ansi?"\e[22m":"") + sprintf("%*s", width-8-strlen(data["header"]["header"]), "") + "  " + right;
            lines += ({ line });
        } else if (arrayp(data["header"]["header"])) {
            if (!data["header"]["columns"]) {
                data["header"]["columns"] = 2;
            }
            columnWidth = arrayp(data["header"]["columns"]) ? sizeof(data["header"]["columns"]) : data["header"]["columns"];
            if (sizeof(data["header"]["header"]) > columnWidth) {
                data["header"]["header"] = data["header"]["header"][0..columnWidth-1];
            }
            line = left + "  " + (ansi?"\e[0;37;40;1m":"") + SEFUN->format_page(data["header"]["header"], data["header"]["columns"], 4, (data["header"]["align"] == "center")) + (ansi?"\e[22m":"") + "  " + right;
            lines += ({ line });
        }
        // Header lines
        if (undefinedp(data["header"]["columns"])) {
            data["header"]["columns"] = 2;
        }
        format = sizeof(data["header"]["items"]) > 0 ? SEFUN->format_page(data["header"]["items"], data["header"]["columns"], 4, (data["header"]["align"] == "center")) : "";
        foreach (string l in explode(format, "\n")) {
            line = left + "  " + l + "  " + right;
            lines += ({ line });
        }
        // Header bottom line
        line = b["v"] + b["bl"] + sprintf("%'"+b["h"]+"'*s", width-4, "") + b["br"] + b["v"];
        if (ansi) {
            if (ansi == "256") {
                line = SEFUN->apply_gradient(line, colors);
            } else {
                line = "\e[36m" + line + "\e[0;37;40m";
            }
        }
        lines += ({ line });
    }

    if (fBody && mapp(data["body"])) {
        data["body"] = ({ data["body"] });
    }
    if (fBody && arrayp(data["body"])) {
        // Body top line
        line = sprintf("%*s", width-2, "");
        if (ansi) {
            if (ansi == "256") {
                line = SEFUN->apply_gradient(line, colors);
            } else {
                line = "\e[36m" + line + "\e[0;37;40m";
            }
        }
        linesBody += ({ line });
        foreach (mapping child in data["body"]) {
            // Body child header
            if (stringp(child["header"])) {
                line = "   " + (ansi?"\e[0;37;40;1m":"") + child["header"] + (ansi?"\e[22m":"") + sprintf("%*s", width-8-strlen(child["header"]), "") + "   ";
                linesBody += ({ line });
            } else if (arrayp(child["header"])) {
                if (!child["columns"]) {
                    child["columns"] = 2;
                }
                columnWidth = arrayp(child["columns"]) ? sizeof(child["columns"]) : child["columns"];
                if (sizeof(child["header"]) > columnWidth) {
                    child["header"] = child["header"][0..columnWidth-1];
                }
                line = "   " + (ansi?"\e[0;37;40;1m":"") + SEFUN->format_page(child["header"], child["columns"], 4, (child["align"] == "center")) + (ansi?"\e[22m":"") + "   ";
                linesBody += ({ line });
            }
            // Body child lines
            if (undefinedp(child["columns"])) {
                child["columns"] = 2;
            }
            format = sizeof(child["items"]) > 0 ? SEFUN->format_page(child["items"], child["columns"], 4, (child["align"] == "center")) : "";
            foreach (string l in explode(format, "\n") - ({""})) {
                line = "   " + l + "   ";
                linesBody += ({ line });
            }
            // Body child bottom line
            line = sprintf("%*s", width-2, "");
            linesBody += ({ line });
        }
        if (ansi && SEFUN->query_account_setting("screenreader") != "on") {
            if (ansi == "256") {
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
                left = "\e[36m" + b["v"] + "\e[0;37;40m";
                right = "\e[36m" + b["v"] + "\e[0;37;40m";
                for (int i = sizeof(linesBody)-1; i >= 0; i --) {
                    linesBody[i] = left + linesBody[i] + right;
                }
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
            if (ansi == "256") {
                left = SEFUN->apply_gradient(left, colors2[0..1]);
                right = SEFUN->apply_gradient(right, colors2[<2..<1]);
            } else {
                left = "\e[36m" + left + "\e[0;37;40m";
                right = "\e[36m" + right + "\e[0;37;40m";
            }
        }

        // Footer top line
        line = b["v"] + b["tl"] + sprintf("%'"+b["h"]+"'*s", width-4, "") + b["tr"] + b["v"];
        if (ansi) {
            if (ansi == "256") {
                line = SEFUN->apply_gradient(line, colors2);
            } else {
                line = "\e[36m" + line + "\e[0;37;40m";
            }
        }
        lines += ({ line });
        // Footer header
        if (stringp(data["footer"]["header"])) {
            line = left + "  " + (ansi?"\e[0;37;40;1m":"") + data["footer"]["header"] + (ansi?"\e[22m":"") + sprintf("%*s", width-8-strlen(data["footer"]["header"]), "") + "  " + right;
            lines += ({ line });
        } else if (arrayp(data["footer"]["header"])) {
            if (!data["footer"]["columns"]) {
                data["footer"]["columns"] = 2;
            }
            columnWidth = arrayp(data["footer"]["columns"]) ? sizeof(data["footer"]["columns"]) : data["footer"]["columns"];
            if (sizeof(data["footer"]["header"]) > columnWidth) {
                data["footer"]["header"] = data["footer"]["header"][0..columnWidth-1];
            }
            line = left + "  " + (ansi?"\e[0;37;40;1m":"") + SEFUN->format_page(data["footer"]["header"], data["footer"]["columns"], 4, (data["footer"]["align"] == "center")) + (ansi?"\e[22m":"") + "  " + right;
            lines += ({ line });
        }
        // Footer lines
        if (undefinedp(data["footer"]["columns"])) {
            data["footer"]["columns"] = 2;
        }
        format = sizeof(data["footer"]["items"]) > 0 ? SEFUN->format_page(data["footer"]["items"], data["footer"]["columns"], 4, (data["footer"]["align"] == "center")) : "";
        foreach (string l in explode(format, "\n")) {
            line = left + "  " + l + "  " + right;
            lines += ({ line });
        }
        // Border Bottom line
        line = b["bl"] + b["b"] + sprintf("%'"+b["h"]+"'*s", width-4, "") + b["b"] + b["br"];
        if (ansi) {
            if (ansi == "256") {
                line = SEFUN->apply_gradient(line, colors2);
            } else {
                line = "\e[36m" + line + "\e[0;37;40m";
            }
        }
        lines += ({ line });
    } else {
        // Border Bottom line
        line = b["bl"] + sprintf("%'"+b["h"]+"'*s", width-2, "") + b["br"];
        if (ansi) {
            if (ansi == "256") {
                line = SEFUN->apply_gradient(line, colors2);
            } else {
                line = "\e[36m" + line + "\e[0;37;40m";
            }
        }
        lines += ({ line });
    }

    return lines;
}

void border (mapping data) {
    SEFUN->this_user()->handle_pager(format_border(data));
}

private varargs string *format_tree (string key, mapping value, mapping b, int indent, int index, int maxIndex, mapping prefix) {
    string *result = ({ }), tmp = "";
    string *ids;
    int i, l;

    if (indent > 0) {
        for (i = 0; i < indent; i ++) {
            if (i > 0) {
                if (prefix[i]) {
                    tmp += "  ";
                } else {
                    tmp += b["v"] + " ";
                }
            }
        }
        if (index == maxIndex) {
            tmp += b["bl"];
            prefix[indent] = 1;
        } else {
            tmp += b["l"];
        }
        tmp += b["h"];
    }

    ids = sort_array(keys(value), (: to_int(explode($1," ")[0]) < to_int(explode($2," ")[0]) ? 0 : 1 :));
    l = sizeof(ids);
    tmp += key;
    indent ++;
    result += ({ tmp });
    for (i = 0; i < l; i ++) {
        result += format_tree(ids[i], value[ids[i]], b, indent, i, l-1, prefix);
        prefix[indent] = 0;
    }

    return result;
}
string *tree (mapping value) {
    string *result = ({ });
    mapping b = query_border_charset();
    string *ids;
    int i, l;

    if (!mapp(value) || !sizeof(value)) error("Bad argument 1 to border->tree");

    ids = sort_array(keys(value), (: to_int(explode($1," ")[0]) < to_int(explode($2," ")[0]) ? 0 : 1 :));
    l = sizeof(ids);
    for (i = 0; i < l; i ++) {
        result += format_tree(ids[i], value[ids[i]], b, 0, i, l-1, ([ ]));
    }

    return result;
}