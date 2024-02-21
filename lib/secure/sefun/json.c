varargs string json_encode (mixed value, mixed *refs) {
    string out = "";
    mixed *tmp;
    int i = 0, l = 0;

    if (!refs) {
        refs = ({ });
    }
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

    if (member_array(value, refs) > -1) {
        return "null";
    }
    if (arrayp(value)) {
        if (l = sizeof(value)) {
            refs += ({ value });
            for (i = 0; i < l; i ++) {
                out += (!i ? "" : ",") + json_encode(value[i], refs);
            }
            return "[" + (sizeof(out) ? out : "") + "]";
        } else {
            return "[]";
        }
    } else if (mapp(value)) {
        tmp = keys(value);
        if (l = sizeof(tmp)) {
            refs += ({ value });
            for (i = 0; i < l; i ++) {
                if (!stringp(tmp[i])) continue;
                out += (!i ? "" : ",") + json_encode(tmp[i], refs) + ":" + json_encode(value[tmp[i]], refs);
            }
        }
        return "{" + (sizeof(out) ? out : "") + "}";
    } else {
        return "null";
    }
}