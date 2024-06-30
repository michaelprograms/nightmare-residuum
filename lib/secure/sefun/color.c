string strip_colour (string str) {
    if (!str) {
        return str;
    }
    // parse blank ANSI color tags
    str = terminal_colour(""+str, D_ANSI->query_unknown_term());
    // strip default ANSI reset color tag added by fluffos
    str = replace_string(str, "\e[49;49m\e[0;10m", "");
    return str;
}

// wrap text, preserving ANSI colors
string wrap_ansi (string str, int width) {
    string *linesANSI, *linesUnknown;
    string strANSI, strUnknown, lastWord;
    int posANSI, posUnknown, i, n, diff;

    strUnknown = terminal_colour(str, D_ANSI->query_unknown_term(), width, 0);
    strUnknown = replace_string(strUnknown, "\e[49;49m\e[0;10m", "");
    strANSI = terminal_colour(str, D_ANSI->query_ansi_term(), width, 0);
    strANSI = replace_string(strANSI, "\e[49;49m\e[0;10m", "");

    linesUnknown = explode(strUnknown, "\n");
    linesANSI = explode(strANSI, "\n");
    n = sizeof(linesANSI);
    for (i = 0; i < n; i ++) {
        if (i >= sizeof(linesUnknown) || !sizeof(linesUnknown[i])) {
            linesANSI[i] = sprintf("%' '*s", width, "");
            continue;
        }

        diff = width - sizeof(linesUnknown[i]);
        if (diff > 0) {
            // when difference in length between ANSI and Unknown,
            // we want to insert padding before the final ANSI tags.
            // find last word in Unknown to the end (includes spaces)
            posUnknown = strsrch(linesUnknown[i], " ", -1);
            lastWord = linesUnknown[i][posUnknown..];
            posUnknown = strsrch(linesUnknown[i], lastWord, -1);
            lastWord = linesUnknown[i][posUnknown..];
            if (posUnknown <= 0) {
                linesANSI[i] += sprintf("%' '*s", diff, "");
            } else {
                posANSI = strsrch(linesANSI[i], lastWord, -1);
                if (posANSI == -1) {
                    linesANSI[i] += sprintf("%' '*s", diff, "");
                } else {
                    linesANSI[i] = linesANSI[i][0..posANSI + sizeof(lastWord)-1] + sprintf("%' '*s", diff, "") + linesANSI[i][posANSI + sizeof(lastWord)..];
                }
            }
            diff = 0;
        }
    }
    return implode(linesANSI, "\n");
}

// Returns a random color triplet
int *query_random_color () {
    int *c = ({ 0, 0, 0, });
    int r = 255;

    c[0] = 64 + random(192);
    r -= (c[0] * 3 / 4);
    c[1] = random(r);
    r -= (c[1] * 3 / 4);
    c[2] = random(r);

    c = shuffle(c);

    return c;
}

// Returns a sRGB value in the range of [0..255] for linear input [0..1]
int color_to_sRGB (float n) {
    float f;

    if (!floatp(n) || undefinedp(n)) {
        error("Bad argument 1 to color->color_to_sRGB");
    }

    if (n <= 0.0031308) {
        f = 12.92 * n;
    } else {
        f = (1.055 * pow(n, 1/2.4)) - 0.055;
    }

    return to_int(255.9999 * f);
}

// Returns a linear value [0..1] for sRGB input [0..255]
float color_from_sRGB (int n) {
    float x, y;

    if (!intp(n) || undefinedp(n)) {
        error("Bad argument 1 to color->color_from_sRGB");
    }

    x = n / 255.0;
    if (x <= 0.04045) {
        y = x / 12.92;
    } else {
        y = pow(((x + 0.055) / 1.055), 2.4);
    }

    return to_float(sprintf("%0.2f", y));
}

// Linear Interpolation a color between two colors at a ratio
float color_lerp (float color1, float color2, float ratio) {
    if (!floatp(color1)) {
        error("Bad argument 1 to color->color_lerp");
    }
    if (!floatp(color2)) {
        error("Bad argument 2 to color->color_lerp");
    }
    if (undefinedp(ratio) || !floatp(ratio) || ratio < 0.0 || ratio > 1.0) {
        error("Bad argument 3 to color->color_lerp");
    }

    return color1 * (1 - ratio) + color2 * ratio;
}

// Generate a color gradient between 'color1' and 'color2' of length 'steps'
// based upon https://stackoverflow.com/questions/22607043/color-gradient-algorithm
string *color_gradient (int *color1, int *color2, int steps) {
    string *gradient = ({ });
    float *c1, *c2, *b;
    float ratio, intensity, *color, total;

    if (!arrayp(color1) && sizeof(color1) != 3) {
        error("Bad argument 1 to color->color_gradient");
    }
    if (!arrayp(color2) && sizeof(color2) != 3) {
        error("Bad argument 2 to color->color_gradient");
    }
    if (!intp(steps) || steps < 2) {
        error("Bad argument 3 to color->color_gradient");
    }

    c1 = ({ 0.0, 0.0, 0.0 });
    c2 = ({ 0.0, 0.0, 0.0 });
    b = ({ 0.0, 0.0 });

    // Red
    c1[0] = color_from_sRGB(color1[0]);
    c2[0] = color_from_sRGB(color2[0]);
    // Green
    c1[1] = color_from_sRGB(color1[1]);
    c2[1] = color_from_sRGB(color2[1]);
    // Blue
    c1[2] = color_from_sRGB(color1[2]);
    c2[2] = color_from_sRGB(color2[2]);
    // Bright
    b[0] = pow(c1[0] + c1[1] + c1[2], 0.43);
    b[1] = pow(c2[0] + c2[1] + c2[2], 0.43);

    for (int i = 0; i < steps; i ++) {
        ratio = i * 1.0 / (steps-1);
        intensity = color_lerp(b[0], b[1], ratio);
        color = ({
            color_lerp(c1[0], c2[0], ratio),
            color_lerp(c1[1], c2[1], ratio),
            color_lerp(c1[2], c2[2], ratio),
        });
        total = color[0] + color[1] + color[2];
        if (total != 0) {
            color[0] = color[0] * intensity / total;
            color[1] = color[1] * intensity / total;
            color[2] = color[2] * intensity / total;
        }
        color = map(color, (: color_to_sRGB :));
        gradient += ({ implode(map(color, (: ""+to_int($1) :)), ";") });
    }

    return gradient;
}

// Applies the array gradient to the string text.
// sizeof(gradient) must equal sizeof(text)
string apply_gradient (string text, string *gradient) {
    string *line, result = "";
    int i, l;

    line = explode(text, "");

    if (sizeof(text) > sizeof(gradient)) {
        error("Bad arguments to color->apply_gradient: invalid sizes "+sizeof(text)+" vs "+sizeof(gradient || ({ })));
    }

    for (i = 0, l = sizeof(line); i < l; i ++) {
        result += "\e[38;2;"+gradient[i]+"m" + line[i];
    }
    result += "\e[0;37;40m";

    return result;
}

string format_message_color (string type, string message) {
    if (type == "say") {
        message = "%^CYAN%^" + replace_string(message, ":", ":%^RESET%^");
    } else if (type == "tell") {
        message = "%^RED%^BOLD%^" + replace_string(message, ":", ":%^RESET%^");
    } else if (type == "go") {
        message = replace_string(message, "%^DIR%^", "%^CYAN%^");
        message = replace_string(message, "%^DEFAULT%^", "%^GREEN%^");
        message = "%^GREEN%^BOLD%^" + message + "%^RESET%^";
    } else if (type == "room listen") {
        message = "%^CYAN%^" + message + "%^RESET%^";
    } else if (type == "room smell") {
        message = "%^ORANGE%^" + message + "%^RESET%^";
    } else if (type == "room living contents") {
        message = "%^RED%^" + replace_string(message, "%^DEFAULT%^", "%^RED%^") + "%^RESET%^";
    } else if (type == "room item contents") {
        message = "%^MAGENTA%^" + replace_string(message, "%^DEFAULT%^", "%^MAGENTA%^") + "%^RESET%^";
    } else if (type == "room exits") {
        message = replace_string(message, "%^DEFAULT%^", "%^GREEN%^");
        message = "%^GREEN%^" + message + "%^RESET%^";
    } else if (type == "channel") {
        message = replace_string(message, "[[", "%^GREEN%^[%^BOLD%^");
        message = replace_string(message, "]]", "%^BOLD_OFF%^GREEN%^]%^RESET%^");
        message = replace_string(message, "((", "%^CYAN%^(%^BOLD%^");
        message = replace_string(message, "))", "%^BOLD_OFF%^CYAN%^)%^RESET%^");
    } else if (type == "channel error") {
        message = replace_string(message, "((", "%^CYAN%^(%^RED%^BOLD%^");
        message = replace_string(message, "))", "%^BOLD_OFF%^CYAN%^)%^RESET%^");
    } else if (type == "attack") {
        message = "%^RED%^BOLD%^" + message + "%^RESET%^";
    } else if (type == "combat hit") {
        message = "%^RED%^" + message + "%^RESET%^";
    } else if (type == "combat miss") {
        message = "%^GREEN%^" + message + "%^RESET%^";
    } else if (type == "combat alert") {
        message = "%^YELLOW%^" + message + "%^RESET%^";
    } else if (type == "ability hit") {
        message = "%^RED%^BOLD%^" + message + "%^RESET%^";
    } else if (type == "ability miss") {
        message = "%^BOLD%^" + message + "%^RESET%^";
    }
    return message;
}