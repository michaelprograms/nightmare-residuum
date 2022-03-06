#include "sefun.h"

varargs string format_header_bar (string title, string optional) {
    int width;
    string bar;

    if (!this_user()->query_account() || this_user()->query_account()->query_setting("screenreader") != "on") {
        width = this_user()->query_account() ? this_user()->query_account()->query_setting("width") : 80;
        bar = "%^RESET%^CYAN%^==";
        if (title) {
            bar += "%^RESET%^/ %^RESET%^BOLD%^" + title;
            if (optional) bar += ":%^RESET%^ " + optional;
            else bar += "%^RESET%^";
            bar += " \\%^CYAN%^=";
        }
        for (int i = strlen(strip_colour(bar)); i < width; i ++) bar += "=";
        bar += "%^RESET%^";
    } else {
        bar = title + (optional ? ": " + optional : "");
    }
    return bar;
}
string format_divider_bar () {
    int width;
    string bar = "";

    if (!this_user()->query_account() || this_user()->query_account()->query_setting("screenreader") != "on") {
        width = this_user()->query_account() ? this_user()->query_account()->query_setting("width") : 80;
        bar = "%^RESET%^BLUE%^-------------------------";
        for (int i = strlen(strip_colour(bar)); i < width; i ++) bar += "-";
        bar += "%^RESET%^";
    }
    return bar;
}
string format_footer_bar () {
    int width;
    string bar = "";

    if (!this_user()->query_account() || this_user()->query_account()->query_setting("screenreader") != "on") {
        width = this_user() ? this_user()->query_account()->query_setting("width") : 80;
        bar = "%^RESET%^CYAN%^=========================";
        for (int i = strlen(strip_colour(bar)); i < width; i ++) bar += "=";
        bar += "%^RESET%^";
    }
    return bar;
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

    for(int i = s; i >= 0; i --) {
        result = digits[i] + (i != s && (s-i)%3 == 0 ? "," : "") + result;
    }

    return (neg ? "-" : "") + result;
}

varargs string format_page (string *items, int columns, int pad) {
    int width, i, j, n = sizeof(items);
    string result = "";

    if (!columns) columns = 2;
    width = this_user() ? this_user()->query_account()->query_setting("width") : 80;
    width = (width / columns) + pad;
    for(i = 0; i < n; i += columns) {
        for (j = 0; j < columns; j ++) {
            if (i + j >= n) break;
            result += pad_right(items[i + j], width); // @TODO for longer strings
        }
        result += "\n";
    }
    return result;
}