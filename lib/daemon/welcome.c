float from_sRGB (int n) {
    float x = n / 255.0,  y;
    if (x <= 0.04045) y = x / 12.92;
    else y = pow(((x + 0.055) / 1.055), 2.4);
    return y;
}
float to_sRGB_f (float n) {
    if (n <= 0.0031308) return 12.92 * n;
    else return (1.055 * pow(n, 1/2.4)) - 0.055;
}
int to_sRGB (float n) {
    return to_int(255.9999 * to_sRGB_f(n));
}
float lerp (float color1, float color2, float ratio) {
    return color1 * (1 - ratio) + color2 * ratio;
}
string *get_color_ratio (int *color1, int *color2, int steps) {
    string *color = ({ });
    float r1 = from_sRGB(color1[0]), g1 = from_sRGB(color1[1]), b1 = from_sRGB(color1[2]);
    float r2 = from_sRGB(color2[0]), g2 = from_sRGB(color2[1]), b2 = from_sRGB(color2[2]);
    float bright1 = pow(r1+g1+b1, 0.43), bright2 = pow(r2+g2+b2, 0.43);
    float intensity;

    for (int i = 0; i < steps; i ++) {
        float *c, ratio = i*1.0/(steps-1), total;
        intensity = lerp(bright1, bright2, ratio);
        c = ({
            lerp(r1, r2, ratio),
            lerp(g1, g2, ratio),
            lerp(b1, b2, ratio),
        });
        total = c[0] + c[1] + c[2];
        if (total != 0) {
            c[0] = c[0] * intensity / total;
            c[1] = c[1] * intensity / total;
            c[2] = c[2] * intensity / total;
        }
        c[0] = to_sRGB(c[0]);
        c[1] = to_sRGB(c[1]);
        c[2] = to_sRGB(c[2]);
        // write("ratio is "+ratio+" intensity: "+intensity+" "+identify(c)+"\n");
        color += ({ to_int(c[0])+";"+to_int(c[1])+";"+to_int(c[2]) });
    }

    return color;
}

string query_banner () {
    int *c1 = ({
        random(255),
        random(255),
        random(255),
    });
    int *c2 = ({
        191,
        191,
        191,
    });
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
        int r = 255;
        int n = 64 + random(192);
        r = r - n;
        c1[0] = n;
        n = random(r);
        r = r - n;
        c1[1] = n;
        n = random(r);
        r = r - n;
        c1[2] = n;
        c1 = shuffle(c1);

        colors += get_color_ratio(c1, c2, 34);
        for(int i = sizeof(colors)-1; i > -1; i --) {
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