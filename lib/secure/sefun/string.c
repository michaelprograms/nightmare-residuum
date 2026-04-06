/**
 * Return a human-readable string representation of any value, suitable for
 * debugging. Strings are quoted and escaped; arrays, mappings, and buffers
 * are formatted with their contents; objects are shown with their file path;
 * undefined values return "UNDEFINED".
 *
 * @param args the value to identify (variadic; pass zero args for UNDEFINED)
 * @returns a string representation of the value
 */
string identify (mixed args...) {
    mixed a;
    int i, l;
    string ret;
    mapping RealMap;

    if (sizeof(args)) {
        a = args[0];
    }

    if ((!undefinedp(a) && intp(a)) || floatp(a)) {
        return "" + a;
    } else if (objectp(a)) {
        if (ret = /** @type {STD_OBJECT} */ (a)->query_key_name()) {
            ret += " ";
        } else {
            ret = "";
        }
        return "OBJ(" + ret + file_name(a) + ")";
    } else if (stringp(a)) {
        a = replace_string(a, "\\", "\\\\");
        a = replace_string(a, "\"", "\\\"");
        a = replace_string(a, "\n", "\\n");
        a = replace_string(a, "\t", "\\t");
        a = replace_string(a, "\r", "\\r");
        return "\"" + a + "\"";
    } else if (mapp(a)) {
        ret = "([ ";
        RealMap = a;
        a = sort_array(keys(RealMap), 1);
        l = sizeof(a);
        for (i = 0 ; i < l ; i ++) {
            if (i) {
                ret += ", ";
            }
            ret += identify(a[i]) + ": " + identify(RealMap[a[i]]);
        }
        return ret + (l ? " " : "") + "])";
    } else if (functionp(a)) {
        return sprintf("%O", a);
    } else if (classp(a)) {
        return replace_string(sprintf("%O", a), "\n", "");
    } else if (arrayp(a)) {
        ret = "({ ";
        l = sizeof(a);
        for (i = 0; i < l; i ++) {
            if (i) {
                ret += ", ";
            }
            ret += identify(a[i]);
        }
        return ret + (l ? " " : "") + "})";
    } else if (bufferp(a)) {
        l = sizeof(a);
        ret = "BUFFER (";
        for (i = 0; i < l; i ++) {
            ret += "0x" + sprintf("%X", a[i]);
            if (i < l-1) {
                ret += ", ";
            }
        }
        ret += ")";
        return ret;
    } else {
        return "UNDEFINED";
    }
}

/**
 * Wrap a string to a given column width, translating %^COLOR%^ tags using
 * the calling object's terminal settings. Strips the default FluffOS ANSI
 * reset sequence appended after each wrap. Returns an empty string when
 * str is falsy or not a string.
 *
 * @param str the string to wrap
 * @param width column width to wrap at; defaults to the caller's "width" setting, or 80
 * @param indent number of spaces to indent continuation lines
 * @param rawANSI when non-zero, pass %^TAG%^ sequences through without translation
 * @returns the wrapped string with color codes resolved
 */
varargs string wrap (string str, int width, int indent, int rawANSI) {
    mapping term;
    object po = previous_object();

    if (!str || !stringp(str)) {
        return "";
    }
    if (undefinedp(width) || width < 0) {
        if (!po || !(width = po->query_setting("width"))) {
            width = 80;
        }
    }

    if (rawANSI) {
        term = ([ ]);
    } else if (po && po->query_setting("ansi") == "on") {
        term = D_ANSI->query_ansi_term();
    } else {
        term = D_ANSI->query_unknown_term();
    }
    str = terminal_colour(str, term, width, indent);
    // strip default ANSI reset color tag added by fluffos
    str = replace_string(str, "\e[49;49m\e[0;10m", "");
    str = str[0..__LARGEST_PRINTABLE_STRING__-1];
    return str;
}

/**
 * Return the number of leading characters that two strings share. Equivalent
 * to finding the length of their longest common prefix.
 *
 * @param a the first string
 * @param b the second string
 * @returns the count of matching leading characters (0 when strings share no prefix)
 */
int string_compare_same_until (string a, string b) {
    int n = 0, l;

    if ((l = strlen(a)) == strlen(b) && a == b) {
        return l;
    }
    if (strlen(a) > strlen(b)) {
        l = strlen(b);
    }

    for (int i = 0; i < l; i ++) {
        if (a[i] != b[i]) {
            break;
        }
        n ++;
    }
    return n;
}

/**
 * Normalize a name for storage or lookup by stripping spaces, apostrophes,
 * and hyphens, then converting to lowercase.
 *
 * @param name the string to sanitize
 * @returns the sanitized name
 */
string sanitize_name (string name) {
    if (undefinedp(name) || !stringp(name)) {
        error("Bad argument 1 to string->sanitize_name");
    }
    name = replace_string(name, " ", "");
    name = replace_string(name, "'", "");
    name = replace_string(name, "-", "");
    return lower_case(name);
}

/**
 * Parse command-line style flags from the beginning of a raw input string.
 * Flags must precede any non-flag words and use one of these formats:
 *
 *   -f          -flag         (boolean, stored as 1)
 *   -f=value    -flag=value   (string value)
 *
 * Parsing stops at the first word that does not begin with "-". The remaining
 * input (after flags) is returned as the first element of the result array.
 * When rawInput is not a string, it is returned as-is with an empty flags map.
 *
 * @param rawInput the raw command input string to parse
 * @returns a two-element array: ({ remaining_input, flags_mapping })
 */
mixed *parse_command_flags (string rawInput) {
    string *args, input = "";
    mapping flags = ([ ]);
    int x, y, l;

    if (!stringp(rawInput)) {
        input = rawInput;
    } else {
        args = explode(rawInput, " ");
        l = sizeof(args);

        for (int i = 0; i < l; i ++) {
            if (regexp(args[i], "^-")) {
                if ((x = strlen(args[i])) > 1) {
                    if ((y = strsrch(args[i], "=")) > -1) {
                        flags[args[i][1..y-1]] = args[i][y+1..];
                    } else {
                        flags[args[i][1..]] = 1;
                    }
                }
            } else {
                input = implode(args[i..], " ");
                break;
            }
        }
    }

    return ({ input, flags });
}