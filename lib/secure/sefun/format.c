varargs string format_page (string *items, int columns, int pad, int center) {
    int width, w, i, j, n, r;
    string *rows = ({});

    if (!arrayp(items) || !sizeof(items)) error("Bad argument 1 to format->format_page");

    if (!columns) columns = 2;
    width = to_int(SEFUN->query_account_setting("width")) - pad * 2;
    w = width / columns; // width of column
    n = sizeof(items); // number of columns
    r = width - (w * columns); // remainder
    items = map(items, (: "" + $1 :));

    for (i = 0; i < n; i += columns) {
        string row = "";
        for (j = 0; j < columns; j ++) {
            string tmp;
            if (i + j >= n) { // ran out of columns to fill final row
                row += sprintf("%' '*s", w*(columns-j), " ");
                break;
            }
            // check text length without ANSI color
            if (sizeof(tmp = SEFUN->strip_colour(items[i + j])) > w) {
                // use stripped text when its longer than w
                if (columns == 1) {
                    string *wrapped = explode(SEFUN->wrap(""+items[i + j], w, 0), "\n");
                    foreach (string line in wrapped) {
                        if (center) {
                            row += sprintf("%|*s", w, ""+line) + "\n";
                        } else {
                            row += sprintf("%-*s", w, ""+line) + "\n";
                        }
                    }
                } else {
                    row += tmp[0..w-1];
                }
            } else {
                // account for any color codes in the text
                int diff = sizeof(items[i+j]) - sizeof(tmp);

                if (center) {
                    row += sprintf("%|*s", w+diff, ""+items[i + j]);
                } else {
                    row += sprintf("%-*s", w+diff, ""+items[i + j]);
                }

                // when color codes present and we're not displaying a RESET, add one
                if (diff > 0 && items[i + j][<9..] != "%^RESET%^") row += "%^RESET%^";
            }
        }
        if (r) row += sprintf("%' '*s", r, "");
        rows += ({ row });
    }
    return implode(rows, "\n");
}

string format_syntax (string text) {
    string s = text;

    if (!stringp(text)) error("Bad argument 1 to format->format_syntax");

    if (!regexp(s, "^<")) s = "<" + s;
    if (!regexp(s, ">$")) s = s + ">";
    if (SEFUN->query_account_setting("ansi") == "on") {
        s = replace_string(s, "<", "%^CYAN%^<");
        s = replace_string(s, "[", "%^BOLD%^[%^BOLD_OFF%^");
        s = replace_string(s, "]", "%^BOLD%^]%^BOLD_OFF%^");
        s = replace_string(s, "(", "%^RESET%^(%^CYAN%^");
        s = replace_string(s, "|", "%^RESET%^|%^CYAN%^");
        s = replace_string(s, ")", "%^RESET%^)%^CYAN%^");
        s = replace_string(s, ">", ">%^RESET%^");
    }
    return s;
}

string format_exit_brief (string dir) {
    string *result = ({});
    if (!stringp(dir)) error("Bad argument 1 to format->format_exit_brief");
    foreach (string part in explode(dir, " ")) {
        switch (part) {
            case "north": result += ({ "n" }); break;
            case "northeast": result += ({ "ne" }); break;
            case "east": result += ({ "e" }); break;
            case "southeast": result += ({ "se" }); break;
            case "south": result += ({ "s" }); break;
            case "southwest": result += ({ "sw" }); break;
            case "west": result += ({ "w" }); break;
            case "northwest": result += ({ "nw" }); break;
            case "up": result += ({ "u" }); break;
            case "down": result += ({ "d" }); break;
            case "enter": result += ({ "ent" }); break;
            default: result += ({ part });
        }
    }
    return implode(result, " ");
}
string format_exit_verbose (string dir) {
    string *result = ({});
    if (!stringp(dir)) error("Bad argument 1 to format->format_exit_verbose");
    foreach (string part in explode(dir, " ")) {
        switch (part) {
            case "n": result += ({ "north" }); break;
            case "ne": result += ({ "northeast" }); break;
            case "e": result += ({ "east" }); break;
            case "se": result += ({ "southeast" }); break;
            case "s": result += ({ "south" }); break;
            case "sw": result += ({ "southwest" }); break;
            case "w": result += ({ "west" }); break;
            case "nw": result += ({ "northwest" }); break;
            case "u": result += ({ "up" }); break;
            case "d": result += ({ "down" }); break;
            case "ent": result += ({ "enter" }); break;
            default: result += ({ part });
        }
    }
    return implode(result, " ");
}
string format_exit_reverse (string dir) {
    string *result = ({});
    if (!stringp(dir)) error("Bad argument 1 to format->format_exit_reverse");
    foreach (string part in explode(dir, " ")) {
        switch (part) {
            case "north": result += ({ "south" }); break;
            case "northeast": result += ({ "southwest" }); break;
            case "east": result += ({ "west" }); break;
            case "southeast": result += ({ "northwest" }); break;
            case "south": result += ({ "north" }); break;
            case "southwest": result += ({ "northeast" }); break;
            case "west": result += ({ "east" }); break;
            case "northwest": result += ({ "southeast" }); break;
            case "up": result += ({ "down" }); break;
            case "down": result += ({ "up" }); break;
            case "enter": result += ({ "out" }); break;
            case "out": result += ({ "enter" }); break;
            default: result += ({ part });
        }
    }
    return implode(result, " ");
}

string format_stat_brief (string stat) {
    string result = "";
    if (!stringp(stat)) error("Bad argument 1 to format->format_stat_brief");
    switch (stat) {
        case "strength": case "str": result = "str"; break;
        case "perception": case "per": result = "per"; break;
        case "endurance": case "end": result = "end"; break;
        case "charisma": case "cha": result = "cha"; break;
        case "intelligence": case "int": result = "int"; break;
        case "agility": case "agi": result = "agi"; break;
        case "luck": case "lck": result = "lck"; break;
    }
    return result;
}
string format_stat_verbose (string stat) {
    string result = "";
    if (!stringp(stat)) error("Bad argument 1 to format->format_stat_verbose");
    switch (stat) {
        case "str": case "strength": result = "strength"; break;
        case "per": case "perception": result = "perception"; break;
        case "end": case "endurance": result = "endurance"; break;
        case "cha": case "charisma": result = "charisma"; break;
        case "int": case "intelligence": result = "intelligence"; break;
        case "agi": case "agility": result = "agility"; break;
        case "lck": case "luck": result = "luck"; break;
    }
    return result;
}

string format_integer (int num) {
    string *digits, result = "";
    int neg, s;

    if (!intp(num)) error("Bad argument 1 to format->format_integer");

    neg = (num < 0);
    num = abs(num);
    digits = explode(""+to_int(num), "");
    s = sizeof(digits) - 1;

    for (int i = s; i >= 0; i --) {
        result = digits[i] + (i != s && (s-i)%3 == 0 ? "," : "") + result;
    }

    return (neg ? "-" : "") + result;
}