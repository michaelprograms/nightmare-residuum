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

// private int parsePos, parseChar, parseLine;

// private varargs int json_decode_parse_at_token (buffer parseText, string token, int start) {
//     int i, j;
//     for (i = start, j = sizeof(token); i < j; i ++) {
//         if (parseText[parsePos + i] != token[i]) {
//             return 0;
//         }
//     }
//     return 1;
// }
// private int json_decode_hexdigit(int ch) {
//     switch(ch) {
//     case '0':
//         return 0;
//     case '1': case '2': case '3': case '4': case '5':
//     case '6': case '7': case '8': case '9':
//         return ch - '0';
//     case 'a': case 'A':
//         return 10;
//     case 'b': case 'B':
//         return 11;
//     case 'c': case 'C':
//         return 12;
//     case 'd': case 'D':
//         return 13;
//     case 'e': case 'E':
//         return 14;
//     case 'f': case 'F':
//         return 15;
//     }
//     return -1;
// }

// private mixed json_decode_parse_string (buffer parseText, int initiator_checked) {
//     int from, to, esc_state, esc_active;
//     string out;
//     if (!initiator_checked) {
//         if (!parseText[parsePos]) {
//             error("Unexpected end of data in json_decode");
//         }
//         if (parseText[parsePos] != '"') {
//             error("Unexpected character in json_decode: " + sprintf("%c", parseText[parsePos]));
//         }
//     }
//     parsePos ++;
//     parseChar ++;
//     from = parsePos;
//     to = -1;
//     esc_state = 0;
//     esc_active = 0;
//     while (to == -1) {
//         switch (parseText[parsePos]) {
//         case 0:
//             error("Unexpected end of data in json_decode");
//         case '\\':
//             esc_state = !esc_state;
//             break;
//         case '"':
//             if (esc_state) {
//                 esc_state = 0;
//                 esc_active ++;
//             } else {
//                 to = parsePos - 1;
//             }
//             break;
//         default         :
//             if (esc_state) {
//                 esc_state = 0;
//                 esc_active ++;
//             }
//             break;
//         }
//         parsePos ++;
//         parseChar ++;
//     }
//     out = string_decode(parseText[from..to], "utf-8");
//     if (esc_active) {
//         // @TODO strsrch swap?
//         if (member_array('"', out) != -1)
//             out = replace_string(out, "\\\"", "\"");
//         if (strsrch(out, "\\b") != -1)
//             out = replace_string(out, "\\b", "\b");
//         if (strsrch(out, "\\f") != -1)
//             out = replace_string(out, "\\f", "\x0c");
//         if (strsrch(out, "\\n") != -1)
//             out = replace_string(out, "\\n", "\n");
//         if (strsrch(out, "\\r") != -1)
//             out = replace_string(out, "\\r", "\r");
//         if (strsrch(out, "\\t") != -1)
//             out = replace_string(out, "\\t", "\t");
//         if (strsrch(out, "\\u") != -1) {
//             for (int i = 0; i < sizeof(out); i ++) {
//                 if (out[i] == '\\' && out[i+1] == 'u') {
//                     int *nybbles = allocate(4);
//                     int character = 0;
//                     i += 2;
//                     for(int k = 0; k < 4; k++) {
//                         if ((nybbles[k] = json_decode_hexdigit(out[i + k])) == -1) {
//                             error("Invalid hex digit in json_decode: " + out[i + k]);
//                         }
//                     }
//                     character = (nybbles[0] << 12) | (nybbles[1] << 8 )| (nybbles[2] << 4) | nybbles[3];
//                     // Single codepoint character
//                     if (!(((character)&0xfffff800)==0xd800)) {
//                         i -= 2;
//                         out[i .. i + 2 + 4 - 1] = sprintf("%c", character);
//                         i = 0;
//                         continue;
//                     } else {
//                         // UTF16 - Surrogate, attempts to parse the second value
//                         int codepoint;
//                         int next_character = 0;
//                         int* nybbles2 = allocate(4);
//                         i += 4;
//                         if (out[i .. i+1] != "\\u") {
//                             error("Invalid string, missing surrogate pair in json_decode");
//                         }
//                         i += 2;
//                         for(int k = 0; k < 4; k++) {
//                             if ((nybbles2[k] = json_decode_hexdigit(out[i + k])) == -1) {
//                                 error("Invalid hex digit in json_decode: " + out[i + k]);
//                             }
//                         }
//                         next_character = (nybbles2[0] << 12) | (nybbles2[1] << 8) | (nybbles2[2] << 4) | (nybbles2[3]);
//                         i -= 2 + 4 + 2; // reset to first \u
//                         codepoint = 0x10000 + (character - 0xd800) * 0x400 + (next_character - 0xDC00);
//                         out[i .. i + 2 + 4 + 2 + 4 - 1] = sprintf("%c", codepoint);
//                         i = 0;
//                         continue;
//                     }
//                 }
//             }
//         }
//         if (member_array('/', out) != -1)
//             out = replace_string(out, "\\/", "/");
//         if (member_array('\\', out) != -1)
//             out = replace_string(out, "\\\\", "\\");
//     }
//     return out;
// }

// private mixed json_decode_parse_value (buffer parseText) {
//     while (1) {
//         switch (parseText[parsePos]) {
//         case 0:
//             error("Unexpected end of data in json_decode");
//         case '{':
//             return "@TODO json_decode_parse_object(parseText)";
//         case '[':
//             return "@TODO json_decode_parse_array(parseText)";
//         case '"':
//             return json_decode_parse_string(parseText, 1);
//         case '-':
//         case '0': case '1': case '2': case '3': case '4':
//         case '5': case '6': case '7': case '8': case '9':
//             return "@TODO json_decode_parse_number(parseText)";
//         case ' ' : case '\t': case '\r':
//             parsePos ++;
//             parseChar ++;
//             break;
//         case 0x0c: case '\n':
//             // @TODO json_decode_parse_next_line(parseText);
//             break;
//         case 't':
//             if (json_decode_parse_at_token(parseText, "true", 1)) {
//                 parsePos += 4;
//                 parseChar += 4;
//                 return 1;
//             } else {
//                 error("Unexpected character in json_decode: " + sprintf("%c", parseText[parsePos]));
//             }
//         case 'f':
//             if (json_decode_parse_at_token(parseText, "false", 1)) {
//                 parsePos += 5;
//                 parseChar += 5;
//                 return 0;
//             } else {
//                 error("Unexpected character in json_decode: " + sprintf("%c", parseText[parsePos]));
//             }
//         case 'n':
//             if (json_decode_parse_at_token(parseText, "null", 1)) {
//                 parsePos += 4;
//                 parseChar += 4;
//                 return 0;
//             } else {
//                 error("Unexpected character in json_decode: " + sprintf("%c", parseText[parsePos]));
//             }
//         default:
//             error("Unexpected character in json_decode: " + sprintf("%c", parseText[parsePos]));
//         }
//     }
// }

// private mixed json_decode_parse (buffer parseText) {
//     mixed out;
//     parsePos = 0;
//     parseChar = 1;
//     parseLine = 1;

//     out = json_decode_parse_value(parseText);

//     write("sefun out = " + SEFUN->identify(out) + "\n");

//     while (1) {
//         switch (parseText[parsePos]) {
//         case 0:
//             return out;
//         case ' ': case '\t': case '\r':
//             parsePos ++;
//             parseChar ++;
//             break;
//         case 0x0c: case '\n':
//             // json_decode_parse_next_line(parse);
//             break;
//         default:
//             error("Unexpected character in json_decode: " + sprintf("%c", parseText[parsePos]));
//         }
//     }
//     return 0;
// }

// varargs string json_decode (mixed value) {
//     buffer parseText;
//     mixed out;

//     if (!value) {
//       return 0;
//     }

//     parseText = string_encode(value, "utf-8") + allocate_buffer(1);

//     // write("json_decode: "+SEFUN->identify(parseText)+"\n");

//     out = json_decode_parse(parseText);
//     return out;
// }



// // -----------

// #define to_string(x)                ("" + (x))

// varargs string json_encode2(mixed value, mixed* pointers) {
//     if (undefinedp(value))
//         return "null";
//     if (intp(value) || floatp(value))
//         return to_string(value);
//     if (stringp(value)) {
//         if (member_array('"', value) != -1)
//             value = replace_string(value, "\"", "\\\"");
//         value = sprintf("\"%s\"", value);
//         if (member_array('\\', value) != -1) {
//             value = replace_string(value, "\\", "\\\\");
//             if (strsrch(value, "\\\"") != -1)
//                 value = replace_string(value, "\\\"", "\"");
//         }
//         if (member_array('\b', value) != -1)
//             value = replace_string(value, "\b", "\\b");
//         if (member_array(0x0c, value) != -1)
//             value = replace_string(value, "\x0c", "\\f");
//         if (member_array('\n', value) != -1)
//             value = replace_string(value, "\n", "\\n");
//         if (member_array('\r', value) != -1)
//             value = replace_string(value, "\r", "\\r");
//         if (member_array('\t', value) != -1)
//             value = replace_string(value, "\t", "\\t");
//         if (member_array(0x1b, value) != -1)
//           value = replace_string(value, "\x1b", "\\u001b");

//         return value;
//     }
//     if (mapp(value)) {
//         string out;
//         int ix = 0;
//         if (pointers) {
//             // Don't recurse into circular data structures, output null for
//             // their interior reference
//             if (member_array(value, pointers) != -1)
//                 return "null";
//             pointers += ({ value });
//         } else {
//             pointers = ({ value });
//         }
//         foreach(mixed k, mixed v in value) {
//             // Non-string keys are skipped because the JSON spec requires that
//             // object field names be strings.
//             if (!stringp(k))
//                 continue;
//             if (ix++)
//                 out = sprintf("%s,%s:%s", out, json_encode2(k, pointers), json_encode2(v, pointers));
//             else
//                 out = sprintf("%s:%s", json_encode2(k, pointers), json_encode2(v, pointers));
//         }
//         if (!out || out == "")
//             return "{}";
//         return sprintf("{%s}", out);
//     }
//     if (arrayp(value))
//     {
//         if (sizeof(value)) {
//             string out;
//             int ix = 0;
//             if (pointers) {
//                 // Don't recurse into circular data structures, output null for
//                 // their interior reference
//                 if (member_array(value, pointers) != -1)
//                     return "null";
//                 pointers += ({ value });
//             } else {
//                 pointers = ({ value });
//             }
//             foreach(mixed v in value)
//                 if (ix++)
//                     out = sprintf("%s,%s", out, json_encode2(v, pointers));
//                 else
//                     out = json_encode2(v, pointers);

//             if (!out || out == "")
//                 return "[]";
//             return sprintf("[%s]", out);
//         } else {
//             return "[]";
//         }
//     }
//     // Values that cannot be represented in JSON are replaced by nulls.
//     return "null";
// }