varargs string json_encode (mixed value) {
    string out = "";
    mixed *tmp;
    int i = 0, l = 0;

    if (undefinedp(value)) {
        out = "null";
    } else if (intp(value) || floatp(value)) {
        out = "" + value;
    } else if (arrayp(value)) {
        if (l = sizeof(value)) {
            for (i = 0; i < l; i ++) {
                out += (!i ? "" : ",") + json_encode(value[i]);
            }
            out = "[" + (sizeof(out) ? out : "") + "]";
        } else {
            out = "[]";
        }
    } else if (mapp(value)) {
        tmp = keys(value);
        l = sizeof(tmp);
        for (i = 0; i < l; i ++) {
            if (!stringp(tmp[i])) {
                continue;
            }
            out += (i > 0 && sizeof(out) ? "," : "") + json_encode(tmp[i]) + ":" + json_encode(value[tmp[i]]);
        }
        out = "{" + (sizeof(out) ? out : "") + "}";
    } else if (stringp(value)) {
        out = value;
        if (strsrch(out, '"') > -1) {
            out = replace_string(out, "\"", "\\\"");
        }
        out = "\"" + out + "\"";
        if (strsrch(out, '\\') > -1) {
            out = replace_string(out, "\\", "\\\\");
            if (strsrch(out, "\\\"") > -1) {
                out = replace_string(out, "\\\"", "\"");
            }
        }
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
    } else {
        out = "null";
    }
    return out;
}

private buffer parseText;
private int parsePos, parseChar, parseLine;

private mixed json_decode_value ();
private mixed json_decode_string (int initiator_checked);

private varargs int json_decode_token (string token, int start) {
    int i, j;
    for (i = start, j = sizeof(token); i < j; i ++) {
        if (parseText[parsePos + i] != token[i]) {
            return 0;
        }
    }
    return 1;
}

private mixed json_decode_object () {
    mapping out = ([ ]);
    int done = 0;
    mixed key, value;
    int found_non_whitespace, found_sep, found_comma;

    parsePos ++;
    parseChar ++;
    if (parseText[parsePos] == '}') {
        done = 1;
        parsePos ++;
        parseChar ++;
    }
    while (!done) {
        found_non_whitespace = 0;
        while (!found_non_whitespace) {
            switch (parseText[parsePos]) {
            case 0:
                error("Unexpected end of data in json_decode_object");
            case ' ': case '\t': case '\r':
                parsePos ++;
                parseChar ++;
                break;
            case 0x0c: case '\n':
                parsePos ++;
                parseLine ++;
                parseChar = 1;
                break;
            default:
                found_non_whitespace = 1;
                break;
            }
        }
        key = json_decode_string(0);
        found_sep = 0;
        while (!found_sep) {
            int ch = parseText[parsePos];
            switch (ch) {
            case 0:
                error("Unexpected end of data in json_decode_object");
            case ':':
                found_sep = 1;
                parsePos ++;
                parseChar ++;
                break;
            case ' ': case '\t': case '\r':
                parsePos ++;
                parseChar ++;
                break;
            case 0x0c: case '\n':
                parsePos ++;
                parseLine ++;
                parseChar = 1;
                break;
            default:
                error("Unexpected character in json_decode_object: " + sprintf("%c", ch));
            }
        }
        value = json_decode_value();
        found_comma = 0;
        while (!found_comma && !done) {
            int ch = parseText[parsePos];
            switch (ch) {
            case 0:
                error("Unexpected end of data in json_decode_object");
            case ',':
                found_comma = 1;
                parsePos ++;
                parseChar ++;
                break;
            case '}':
                done = 1;
                parsePos ++;
                parseChar ++;
                break;
            case ' ': case '\t': case '\r':
                parsePos ++;
                parseChar ++;
                break;
            case 0x0c: case '\n':
                parsePos ++;
                parseLine ++;
                parseChar = 1;
                break;
            default:
                error("Unexpected character in json_decode_object: " + sprintf("%c", ch));
            }
        }
        out[key] = value;
    }
    return out;
}
private mixed json_decode_array () {
    mixed *out = ({ });
    int done = 0;
    int found_comma;
    int ch;
    mixed value;
    parsePos ++;
    parseChar ++;
    ch = parseText[parsePos];
    if (ch == ']') {
        done = 1;
        parsePos ++;
        parseChar ++;
    }
    while (!done) {
        value = json_decode_value();
        found_comma = 0;
        while (!found_comma && !done) {
            ch = parseText[parsePos];
            switch (ch) {
            case 0:
                error("Unexpected end of data in json_decode_array");
            case ',':
                found_comma = 1;
                parsePos ++;
                parseChar ++;
                break;
            case ']':
                done = 1;
                parsePos ++;
                parseChar ++;
                break;
            case ' ': case '\t': case '\r':
                parsePos ++;
                parseChar ++;
                break;
            case 0x0c: case '\n':
                parsePos ++;
                parseLine ++;
                parseChar = 1;
                break;
            default:
                error("Unexpected character in json_decode_array: " + sprintf("%c", ch));
            }
        }
        out += ({ value });
    }
    return out;
}

private mixed json_decode_string (int initiator_checked) {
    int from, to, esc_state, esc_active;
    string out;
    int *nybbles = allocate(4);
    int character = 0;
    int codepoint;
    int next_character = 0;
    if (!initiator_checked) {
        if (!parseText[parsePos]) {
            error("Unexpected end of data in json_decode_array");
        }
        if (parseText[parsePos] != '"') {
            error("Unexpected character in json_decode: " + sprintf("%c", parseText[parsePos]));
        }
    }
    parsePos ++;
    parseChar ++;
    from = parsePos;
    to = -1;
    esc_state = 0;
    esc_active = 0;
    while (to == -1) {
        switch (parseText[parsePos]) {
        case 0:
            error("Unexpected end of data in json_decode_array");
        case '\\':
            esc_state = !esc_state;
            break;
        case '"':
            if (esc_state) {
                esc_state = 0;
                esc_active ++;
            } else {
                to = parsePos - 1;
            }
            break;
        default:
            if (esc_state) {
                esc_state = 0;
                esc_active ++;
            }
            break;
        }
        parsePos ++;
        parseChar ++;
    }
    out = string_decode(parseText[from..to], "utf-8");
    if (esc_active) {
        if (strsrch(out, '"') != -1) {
            out = replace_string(out, "\\\"", "\"");
        }
        if (strsrch(out, "\\b") != -1) {
            out = replace_string(out, "\\b", "\b");
        }
        if (strsrch(out, "\\f") != -1) {
            out = replace_string(out, "\\f", "\x0c");
        }
        if (strsrch(out, "\\n") != -1) {
            out = replace_string(out, "\\n", "\n");
        }
        if (strsrch(out, "\\r") != -1) {
            out = replace_string(out, "\\r", "\r");
        }
        if (strsrch(out, "\\t") != -1) {
            out = replace_string(out, "\\t", "\t");
        }
        if (strsrch(out, "\\u") != -1) {
            for (int i = 0; i < sizeof(out); i ++) {
                if (out[i] == '\\' && out[i+1] == 'u') {
                    i += 2;
                    for (int k = 0; k < 4; k++) {
                        if (undefinedp(nybbles[k] = SEFUN->hex_to_int(out[i+k..i+k]))) {
                            error("Invalid hex digit in json_decode_array: " + out[i+k]);
                        }
                    }
                    character = (nybbles[0] << 12) | (nybbles[1] << 8) | (nybbles[2] << 4) | nybbles[3];
                    // Single codepoint character
                    if (!(((character)&0xfffff800) == 0xd800)) {
                        i -= 2;
                        out[i .. i + 2 + 4 - 1] = sprintf("%c", character);
                        i = 0;
                        continue;
                    } else {
                        nybbles = allocate(4);
                        // UTF16 - Surrogate, attempts to parse the second value
                        i += 4;
                        if (out[i .. i+1] != "\\u") {
                            error("Invalid string, missing surrogate pair in json_decode_array");
                        }
                        i += 2;
                        for (int k = 0; k < 4; k ++) {
                            if (undefinedp(nybbles[k] = SEFUN->hex_to_int(out[i+k..i+k]))) {
                                error("Invalid hex digit in json_decode_array: " + out[i+k]);
                            }
                        }
                        next_character = (nybbles[0] << 12) | (nybbles[1] << 8) | (nybbles[2] << 4) | (nybbles[3]);
                        i -= 2 + 4 + 2; // reset to first \u
                        codepoint = 0x10000 + (character - 0xd800) * 0x400 + (next_character - 0xDC00);
                        out[i..i+2+4+2+4-1] = sprintf("%c", codepoint);
                        i = 0;
                        continue;
                    }
                }
            }
        }
        if (strsrch(out, '/') != -1) {
            out = replace_string(out, "\\/", "/");
        }
        if (strsrch(out, '\\') != -1) {
            out = replace_string(out, "\\\\", "\\");
        }
    }
    return out;
}

private mixed json_decode_number () {
    int from = parsePos;
    int to = -1;
    int dot = -1;
    int exp = -1;
    int ch;
    int next_ch;
    string number;

    ch = parseText[parsePos];
    if (ch == '-') {
        next_ch = parseText[parsePos + 1];
        if (!next_ch) {
            error("Unexpected end of data in json_decode_number");
        }
        if (next_ch < '0' || next_ch > '9') {
            error("Unexpected character in json_decode_number: " + sprintf("%c", next_ch));
        }
        parsePos ++;
        parseChar ++;
    }

    ch = parseText[parsePos];
    if (ch == '0') {
        // 0 can only either be an direct int value 0, or 0e or 0E
        next_ch = parseText[parsePos + 1];
        // 0 before EOF
        if (next_ch == 0) {
            parsePos ++;
            parseChar ++;
            return 0;
        }
        // only valid char here are .eE, continue parse
        if (next_ch == '.' || next_ch == 'e' || next_ch == 'E') {
            parsePos ++;
            parseChar ++;
        } else {
            // consume until next non-whitespace
            while (1) {
                parsePos ++;
                parseChar ++;
                ch = parseText[parsePos];
                if (ch != ' ' && ch != '\n' && ch != '\r' && ch != '\t') {
                    return;
                }
            }
            next_ch = parseText[parsePos];
            // can not continue to be number.
            if ((next_ch >= '0' && next_ch <= '9') || next_ch == '-') {
                error("Unexpected character in json_decode_number: " + sprintf("%c", next_ch));
            }
            return 0;
        }
    }
    while (to == -1) {
        ch = parseText[parsePos];
        switch (ch) {
        case '.':
            if (dot != -1 || exp != -1) {
                error("Unexpected character in json_decode_number: " + sprintf("%c", ch));
            }
            dot = parsePos;
            parsePos ++;
            parseChar ++;
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            parsePos ++;
            parseChar ++;
            break;
        case 'e': case 'E':
            if (exp != -1) {
                error("Unexpected character in json_decode_number: " + sprintf("%c", ch));
            }
            exp = parsePos;
            parsePos ++;
            parseChar ++;
            break;
        case '-': case '+':
            if (exp == parsePos - 1) {
                parsePos ++;
                parseChar ++;
                break;
            }
            // Fallthrough
        default:
            to = parsePos - 1;
            if (dot == to || to < from) {
                error("Unexpected character in json_decode_number: " + sprintf("%c", ch));
            }
            break;
        }
    }
    number = string_decode(parseText[from..to], "utf-8");
    if (dot != -1 || exp != -1) {
        return to_float(number);
    } else {
        return to_int(number);
    }
}

private mixed json_decode_value () {
    while (1) {
        switch (parseText[parsePos]) {
        case 0:
            error("Unexpected end of data in json_decode");
        case '{':
            return json_decode_object();
        case '[':
            return json_decode_array();
        case '"':
            return json_decode_string(1);
        case '-':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return json_decode_number();
        case ' ': case '\t': case '\r':
            parsePos ++;
            parseChar ++;
            break;
        case 0x0c: case '\n':
            parsePos ++;
            parseLine ++;
            parseChar = 1;
            break;
        case 't':
            if (json_decode_token("true", 1)) {
                parsePos += 4;
                parseChar += 4;
                return 1;
            } else {
                error("Unexpected character in json_decode_value: " + sprintf("%c", parseText[parsePos]));
            }
        case 'f':
            if (json_decode_token("false", 1)) {
                parsePos += 5;
                parseChar += 5;
                return 0;
            } else {
                error("Unexpected character in json_decode_value: " + sprintf("%c", parseText[parsePos]));
            }
        case 'n':
            if (json_decode_token("null", 1)) {
                parsePos += 4;
                parseChar += 4;
                return 0;
            } else {
                error("Unexpected character in json_decode_value: " + sprintf("%c", parseText[parsePos]));
            }
        default:
            error("Unexpected character in json_decode_value: " + sprintf("%c", parseText[parsePos]));
        }
    }
}

varargs string json_decode (mixed value) {
    mixed out;

    if (!value) {
        return 0;
    }
    parseText = string_encode(value, "utf-8") + allocate_buffer(1);
    parsePos = 0;
    parseChar = 1;
    parseLine = 1;
    out = json_decode_value();
    while (1) {
        switch (parseText[parsePos]) {
        case 0:
            return out;
        case ' ': case '\t': case '\r':
            parsePos ++;
            parseChar ++;
            break;
        case 0x0c: case '\n':
            parsePos ++;
            parseLine ++;
            parseChar = 1;
            break;
        default:
            error("Unexpected character in json_decode: " + sprintf("%c", parseText[parsePos]));
        }
    }
    return 0;
}