varargs string json_encode (mixed value) {
    string out = "";
    mixed *tmp;
    int i = 0, l = 0;

    if (undefinedp(value)) {
        return "null";
    } else if (intp(value) || floatp(value)) {
        return "" + value;
    } else if (stringp(value)) {
        out = value;
        if (strsrch(out, '\\') > -1) {
            out = replace_string(out, "\\", "\\\\");
            if (strsrch(out, "\\\"") > -1) {
                out = replace_string(out, "\\\"", "\"");
            }
        }
        out = "\"" + out + "\"";
        if (strsrch(out, '\b') > -1) {
            out = replace_string(out, "\b", "\\b");
        }
        if (strsrch(out, '\n') > -1) {
            out = replace_string(out, "\n", "\\n");
        }
        if (strsrch(out, '\r') > -1) {
            out = replace_string(out, "\r", "\\r");
        }
        if (strsrch(out, '\t') > -1) {
            out = replace_string(out, "\t", "\\t");
        }
        if (member_array(0x0c, out) > -1) {
            out = replace_string(out, "\x0c", "\\f");
        }
        if (member_array(0x1b, out) > -1) {
            out = replace_string(out, "\x1b", "\\u001b");
        }
        return out;
    }
    if (arrayp(value)) {
        if (l = sizeof(value)) {
            for (i = 0; i < l; i ++) {
                out += (!i ? "" : ",") + json_encode(value[i]);
            }
            return "[" + (sizeof(out) ? out : "") + "]";
        } else {
            return "[]";
        }
    } else if (mapp(value)) {
        tmp = keys(value);
        if (l = sizeof(tmp)) {
            for (i = 0; i < l; i ++) {
                if (!stringp(tmp[i])) {
                    continue;
                }
                out += (i > 0 && sizeof(out) ? "," : "") + json_encode(tmp[i]) + ":" + json_encode(value[tmp[i]]);
            }
        }
        return "{" + (sizeof(out) ? out : "") + "}";
    } else {
        return "null";
    }
}