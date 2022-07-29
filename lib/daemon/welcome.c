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
        colors += color_gradient(c1, c2, 34);
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