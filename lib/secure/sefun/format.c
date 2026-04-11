#include <config.h>

/**
 * Format an array of strings into a paged column grid. Each item fills one
 * cell; rows wrap automatically. Items longer than their column width are
 * truncated (multi-column) or word-wrapped (single-column).
 *
 * @param items the strings to display; numbers are coerced to strings
 * @param columns either an int (equal-width columns) or an int array of column width ratios
 * @param pad padding to subtract from each side of the total width
 * @param center when non-zero, center-align text within each column
 * @param ansi when set, use ANSI-aware wrapping for single-column overflow
 * @returns the formatted grid as a newline-delimited string
 */
varargs string format_page (string *items, mixed columns, int pad, int center, string ansi) {
    int totalWidth, width, numItems, numColumns, remainder, ratioSum;
    int i, j, diff;
    string *rows = ({ }), row, tmp, *wrapped, format;

    if (!arrayp(items) || !sizeof(items)) {
        error("Bad argument 1 to format->format_page");
    }

    if (arrayp(columns)) {
        numColumns = sizeof(columns);
        for (i = 0; i < numColumns; i ++) {
            ratioSum += columns[i]; // calculate total columns ratios
        }
    } else if (intp(columns) && !undefinedp(columns)) {
        numColumns = columns;
    } else {
        error("Bad argument 2 to format->format_page");
    }
    if (numColumns < 1) {
        error("Bad argument 2 to format->format_page");
    }

    totalWidth = (to_int(SEFUN->query_account_setting("width")) || DEFAULT_WIDTH) - pad * 2;
    items = map(items, (: "" + $1 :)); // force numbers to string
    numItems = sizeof(items);
    format = "%" + (center?"|":"-") + "*s";

    for (i = 0; i < numItems; i += numColumns) {
        row = "";

        if (ratioSum) {
            // reset remainder to totalWidth
            remainder = totalWidth;
        } else {
            // determine width of column and remainder
            width = totalWidth / numColumns;
            remainder = totalWidth - (width * numColumns);
        }

        for (j = 0; j < numColumns; j ++) {
            if (ratioSum) {
                // calculate this column's width
                width = totalWidth / ratioSum * columns[j];
                remainder -= width;
            }

            if (i + j >= numItems) {
                // ran out of columns to fill final row
                row += sprintf("%' '*s", width * (numColumns - j), " ");
                break;
            }

            // check text length without ANSI color
            if (sizeof(tmp = SEFUN->strip_colour(items[i+j])) > width) {
                // use stripped text when its longer than width
                if (numColumns == 1) {
                    // check text is all the same character
                    if (pcre_match(items[i+j], "^(.)\\1*$")) {
                        items[i+j] = items[i+j][0..width-1];
                    }
                    if (ansi) {
                        wrapped = explode(SEFUN->wrap_ansi(items[i+j], width), "\n");
                    } else {
                        wrapped = explode(SEFUN->wrap(items[i+j], width), "\n");
                    }
                    foreach (string line in wrapped) {
                        row += sprintf(format, width, "" + line) + "\n";
                    }
                } else {
                    row += tmp[0..width-1];
                }
            } else {
                // account for any color codes in the text
                diff = sizeof(items[i+j]) - sizeof(tmp);
                row += sprintf(format, width + diff, items[i+j]);
                // when color codes present and we're not displaying a RESET, add one
                if (diff > 0 && items[i+j][<9..] != "%^RESET%^") {
                    row += "%^RESET%^";
                }
            }
        }
        if (remainder) {
            row += sprintf("%*s", remainder, "");
        }
        rows += ({ row });
    }
    return implode(rows, "\n");
}

/**
 * Wrap a command syntax string in angle brackets and apply ANSI coloring
 * when the player's ansi setting is "on". Square brackets, parentheses, and
 * pipe characters each receive distinct colors to aid readability.
 *
 * @param text the raw syntax string; returns 0 if not a string
 * @returns the formatted syntax string, with or without ANSI codes
 */
string format_syntax (string text) {
    string s;

    if (!stringp(text)) {
        return 0;
    }
    s = text;
    if (!regexp(s, "^<")) {
        s = "<" + s;
    }
    if (!regexp(s, ">$")) {
        s = s + ">";
    }
    if (SEFUN->query_account_setting("ansi") == "on") {
        s = replace_string(s, "<", "%^CYAN%^<%^I_CYAN%^");
        s = replace_string(s, "[", "%^BOLD%^[%^BOLD_OFF%^");
        s = replace_string(s, "]", "%^BOLD%^]%^BOLD_OFF%^");
        s = replace_string(s, "(", "%^RESET%^(%^CYAN%^");
        s = replace_string(s, "|", "%^RESET%^|%^CYAN%^");
        s = replace_string(s, ")", "%^RESET%^)%^I_CYAN%^");
        s = replace_string(s, ">", "%^CYAN%^>%^RESET%^");
        s = replace_string(s, "%^I_CYAN%^%^CYAN%^", "%^CYAN%^");
    }
    return s;
}

/**
 * Convert a direction string from verbose form to abbreviated form
 * (ex: "north" to "n", "northeast" to "ne", "enter" to "ent"). Unknown
 * words are passed through unchanged. Handles multi-word directions.
 *
 * @param dir the direction string to abbreviate
 * @returns the abbreviated direction string
 */
string format_exit_brief (string dir) {
    string *result = ({ });
    foreach (string part in explode(dir || "", " ")) {
        switch (part) {
        case "north":
            result += ({ "n" });
            break;
        case "northeast":
            result += ({ "ne" });
            break;
        case "east":
            result += ({ "e" });
            break;
        case "southeast":
            result += ({ "se" });
            break;
        case "south":
            result += ({ "s" });
            break;
        case "southwest":
            result += ({ "sw" });
            break;
        case "west":
            result += ({ "w" });
            break;
        case "northwest":
            result += ({ "nw" });
            break;
        case "up":
            result += ({ "u" });
            break;
        case "down":
            result += ({ "d" });
            break;
        case "enter":
            result += ({ "ent" });
            break;
        default:
            result += ({ part });
        }
    }
    return implode(result, " ");
}

/**
 * Convert a direction string from abbreviated form to verbose form
 * (ex: "n" to "north", "ne" to "northeast", "ent" to "enter"). Unknown
 * words are passed through unchanged. Handles multi-word directions.
 *
 * @param dir the direction string to expand
 * @returns the verbose direction string
 */
string format_exit_verbose (string dir) {
    string *result = ({ });
    foreach (string part in explode(dir || "", " ")) {
        switch (part) {
        case "n":
            result += ({ "north" });
            break;
        case "ne":
            result += ({ "northeast" });
            break;
        case "e":
            result += ({ "east" });
            break;
        case "se":
            result += ({ "southeast" });
            break;
        case "s":
            result += ({ "south" });
            break;
        case "sw":
            result += ({ "southwest" });
            break;
        case "w":
            result += ({ "west" });
            break;
        case "nw":
            result += ({ "northwest" });
            break;
        case "u":
            result += ({ "up" });
            break;
        case "d":
            result += ({ "down" });
            break;
        case "ent":
            result += ({ "enter" });
            break;
        default:
            result += ({ part });
        }
    }
    return implode(result, " ");
}

/**
 * Convert a direction string to its opposite (ex: "north" to "south",
 * "enter" to "out"). Unknown words are passed through unchanged. Handles
 * multi-word directions.
 *
 * @param dir the direction string to reverse
 * @returns the reversed direction string
 */
string format_exit_reverse (string dir) {
    string *result = ({ });
    foreach (string part in explode(dir || "", " ")) {
        switch (part) {
        case "north":
            result += ({ "south" });
            break;
        case "northeast":
            result += ({ "southwest" });
            break;
        case "east":
            result += ({ "west" });
            break;
        case "southeast":
            result += ({ "northwest" });
            break;
        case "south":
            result += ({ "north" });
            break;
        case "southwest":
            result += ({ "northeast" });
            break;
        case "west":
            result += ({ "east" });
            break;
        case "northwest":
            result += ({ "southeast" });
            break;
        case "up":
            result += ({ "down" });
            break;
        case "down":
            result += ({ "up" });
            break;
        case "enter":
            result += ({ "out" });
            break;
        case "out":
            result += ({ "enter" });
            break;
        default:
            result += ({ part });
        }
    }
    return implode(result, " ");
}

/**
 * Convert a stat name to its 3-letter abbreviation (ex: "strength" to "str").
 * Accepts either form as input. Returns "" for unrecognized values.
 *
 * @param stat the stat name or abbreviation
 * @returns the 3-letter abbreviation, or "" if unrecognized
 */
string format_stat_brief (string stat) {
    string result = "";
    switch (stat) {
    case "strength": case "str":
        result = "str";
        break;
    case "perception": case "per":
        result = "per";
        break;
    case "endurance": case "end":
        result = "end";
        break;
    case "charisma": case "cha":
        result = "cha";
        break;
    case "intelligence": case "int":
        result = "int";
        break;
    case "agility": case "agi":
        result = "agi";
        break;
    case "luck": case "lck":
        result = "lck";
        break;
    }
    return result;
}

/**
 * Convert a stat abbreviation to its full name (ex: "str" to "strength").
 * Accepts either form as input. Returns "" for unrecognized values.
 *
 * @param stat the stat abbreviation or full name
 * @returns the full stat name, or "" if unrecognized
 */
string format_stat_verbose (string stat) {
    string result = "";
    switch (stat) {
    case "str": case "strength":
        result = "strength";
        break;
    case "per": case "perception":
        result = "perception";
        break;
    case "end": case "endurance":
        result = "endurance";
        break;
    case "cha": case "charisma":
        result = "charisma";
        break;
    case "int": case "intelligence":
        result = "intelligence";
        break;
    case "agi": case "agility":
        result = "agility";
        break;
    case "lck": case "luck":
        result = "luck";
        break;
    }
    return result;
}

/**
 * Format an integer with comma separators (ex: 1234567 to "1,234,567").
 * Handles negative numbers. Non-integer or undefined values are treated as 0.
 *
 * @param num the integer to format
 * @returns the formatted string with comma separators
 */
string format_integer (int num) {
    string *digits, result = "";
    int neg, s;

    if (undefinedp(num) || !intp(num)) {
        num = 0;
    }

    neg = (num < 0);
    num = abs(num);
    digits = explode(""+to_int(num), "");
    s = sizeof(digits) - 1;

    for (int i = s; i >= 0; i --) {
        result = digits[i] + (i != s && (s-i)%3 == 0 ? "," : "") + result;
    }

    return (neg ? "-" : "") + result;
}