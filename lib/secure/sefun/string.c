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
        if (ret = a->query_key_name()) {
            ret += " ";
        } else {
            ret = "";
        }
        return "OBJ(" + ret + file_name(a) + ")";
    } else if (stringp(a)) {
        a = replace_string(a, "\"", "\\\"");
        a = "\"" + a + "\"";
        a = replace_string(a, "\\", "\\\\");
        a = replace_string(a, "\\\"", "\"");
        a = replace_string(a, "\n", "\\n");
        a = replace_string(a, "\t", "\\t");
        return a;
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

varargs string wrap (string str, int width, int indent, int rawANSI) {
    mapping term;
    object po = previous_object();

    if (!str || !stringp(str)) return "";
    if (undefinedp(width) || width < 0) {
        if (!(width = po->query_setting("width"))) {
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
    if (strlen(str) > __LARGEST_PRINTABLE_STRING__) {
        str = str[0..__LARGEST_PRINTABLE_STRING__-1];
    }
    return str;
}

int string_compare_same_until (string a, string b) {
    // Returns n where a[0..n] == b[0..n]
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

string sanitize_name (string name) {
    if (undefinedp(name) || !stringp(name)) {
        error("Bad argument 1 to string->sanitize_name");
    }
    name = replace_string(name, " ", "");
    name = replace_string(name, "'", "");
    name = replace_string(name, "-", "");
    return lower_case(name);
}

// Parse the raw input for any command line flags provided
// Flags must be at the beginning of raw input and in format:
// -f           or      -flag
// -f=text      or      -flag=text
// -f=123.0     or      -flag=123.0
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