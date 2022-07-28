string *get_color_ratio (int *color1, int *color2, int steps) {
    string *color = ({ });
    float r1 = color_from_sRGB(color1[0]), g1 = color_from_sRGB(color1[1]), b1 = color_from_sRGB(color1[2]);
    float r2 = color_from_sRGB(color2[0]), g2 = color_from_sRGB(color2[1]), b2 = color_from_sRGB(color2[2]);
    float bright1 = pow(r1+g1+b1, 0.43), bright2 = pow(r2+g2+b2, 0.43);
    float intensity;

    for (int i = 0; i < steps; i ++) {
        float *c, ratio, total;

        ratio = i * 1.0/(steps-1);
        intensity = color_lerp(bright1, bright2, ratio);
        c = ({
            color_lerp(r1, r2, ratio),
            color_lerp(g1, g2, ratio),
            color_lerp(b1, b2, ratio),
        });
        total = c[0] + c[1] + c[2];
        if (total != 0) {
            c[0] = c[0] * intensity / total;
            c[1] = c[1] * intensity / total;
            c[2] = c[2] * intensity / total;
        }
        c[0] = color_to_sRGB(c[0]);
        c[1] = color_to_sRGB(c[1]);
        c[2] = color_to_sRGB(c[2]);
        color += ({ implode(map(c, (: ""+to_int($1) :)), ";") });
    }

    return color;
}

string query_banner () {
    string *lines;
    string text = "\e[0;37;40m"; // start with ANSI reset
    string *colors = allocate(6); // left padding

    int clump = 0;
    string tmp;
    int pad;

    lines = explode(read_file("/etc/welcome"), "\n");
    tmp = " " + mud_name() + " ";
    pad = 40-strlen(tmp)/2;
    lines[<2] = lines[<2][0..pad-1] + tmp + lines[<2][80-pad..79];

    tmp = " Driver: " + driver_version() + "   Mudlib: " + mudlib_version() + " ";
    pad = 40-strlen(tmp)/2;
    lines[<1] = lines[<1][0..pad-1] + tmp + lines[<1][80-pad+strlen(tmp)%2..79];

    if (previous_object() && previous_object()->query_terminal_color() == "256") {
        int *c1 = query_random_color();
        int *c2 = ({ 191, 191, 191, });
        colors += get_color_ratio(c1, c2, 34);
        for (int i = sizeof(colors)-1; i > -1; i --) {
            colors[i] = "\e[38;2;"+colors[i]+"m";
            colors += ({ colors[i] });
        }
    } else { // 16 bit mode
        colors += allocate(68, "\e[33m");
    }

    for (int i = 0; i < sizeof(lines); i ++) {
        for (int j = 0; j < sizeof(lines[i]); j ++) {
            if (i == sizeof(lines) - 1 && j >= pad && j <= strlen(lines[i])-pad) {
                text += lines[i][j..j]; // preserve dots in driver/mudlib versions
            } else if (lines[i][j..j] == ".") {
                if (clump > 0) {
                    clump --;
                    text += " ";
                } else if (random(16)) {
                    text += " ";
                } else {
                    clump = 4;
                    text += "\e[0;37;40m.";
                }
            } else if (lines[i][j..j] != " ") {
                if (i > 0 && i < 8) text += colors[j];
                text += lines[i][j..j] + "\e[0;37;40m";
            } else text += " ";
        }
        text += "\n";
    }
    return text;
}