private string *__Banner = ({
    "................................................................................",
    "................................................................................",
    "........  ______ _______ _______ _____ ______  _ ... _ _ ... _ _______ .........",
    "........ |_____/ |______ |______ . | . | ... \\ | ... | | ... | |  |  | .........",
    "........ |    \\_ |______ ______| __|__ |_____/ |_____| |_____| |  |  | .........",
    "................................................................................",
    "................................................................................",
    "................................................................................",
    "................................................................................",
});

string query_banner () {
    string text = "\e[0;37;40m"; // start with ANSI reset
    string *colors = allocate(6); // left padding
    object tu = SEFUN->this_user();
    string userColor = tu && tu->query_terminal("color");

    string tmp, dot = " ";
    int pad;
    int r = 25 + 1+random(100);

    tmp = "  " + SEFUN->mud_name() + "  ";
    pad = 40 - sizeof(tmp) / 2;
    __Banner[<2] = __Banner[<2][0..pad-1] + tmp + __Banner[<2][80-pad..79];

    tmp = "  " + SEFUN->version() + "   " + SEFUN->mudlib_version() + "  ";
    pad = 40 - sizeof(tmp) / 2;
    __Banner[<1] = __Banner[<1][0..pad-1] + tmp + __Banner[<1][80-pad+sizeof(tmp)%2..79];

    if (userColor == "256") {
        int *c1 = SEFUN->query_random_color();
        int *c2 = allocate(3, 159+random(32));
        colors += SEFUN->color_gradient(c1, c2, 34);
        for (int i = sizeof(colors)-1; i > -1; i --) {
            colors[i] = "\e[38;2;"+colors[i]+"m";
            colors += ({ colors[i] });
        }
    } else { // 16 bit mode
        colors += allocate(68, "\e[33m");
    }

    for (int i = 0; i < sizeof(__Banner); i ++) {
        for (int j = 0; j < sizeof(__Banner[i]); j ++) {
            if (i == sizeof(__Banner) - 1 && j >= pad && j <= sizeof(__Banner[i])-pad-1) {
                text += __Banner[i][j..j]; // preserve dots in driver/mudlib versions
            } else if (__Banner[i][j..j] == ".") {
                if (userColor == "256") {
                    dot[0] = 10240 + (
                        (!random(r) ? 0x1  : 0) |
                        (!random(r) ? 0x2  : 0) |
                        (!random(r) ? 0x4  : 0) |
                        (!random(r) ? 0x40 : 0) |
                        (!random(r) ? 0x8  : 0) |
                        (!random(r) ? 0x10 : 0) |
                        (!random(r) ? 0x20 : 0) |
                        (!random(r) ? 0x80 : 0)
                    );
                    if (dot[0] == 10240) {
                        dot = " ";
                    }
                } else {
                    dot = random(r/2) ? " " : ".";
                }
                text += "\e[0;37;40m" + dot;
            } else if (__Banner[i][j..j] != " ") {
                if (i > 0 && i < 8) {
                    text += colors[j];
                }
                text += __Banner[i][j..j] + "\e[0;37;40m";
            } else {
                text += " ";
            }
        }
        text += "\n";
    }
    return text;
}