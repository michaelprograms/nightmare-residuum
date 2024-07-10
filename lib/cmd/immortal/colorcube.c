inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("colorcube");
    set_help_text("The colorcube command displays the cube of xterm colors.");
}

void command (string input, mapping flags) {
    string result = "";
    string line;

    for (int y = 0; y < 6; y ++) {
        line = "";
        for (int x = 0; x < 6; x ++) {
            for (int z = 0; z < 6; z ++) {
                int X, Y, Z;
                if (x % 2 == 0) {
                    X = x;
                    Y = y;
                    Z = z;
                } else {
                    X = x;
                    Y = y;
                    Z = 5 - z;
                }
                if (y == 0) write(x+" "+y+" "+z+" -> " + x+" "+Y+" "+Z + " "+sprintf("%%^B_%c%c%c%%^  ", 'A' + x, 'A' + y, 'A' + z)+"%^RESET%^\n");
                // write(sprintf("%c", 'A' + x)+"\n");
                line += sprintf("%%^B_%c%c%c%%^  ", 'A' + X, 'A' + Y, 'A' + Z);
            }
            if (x % 2) line += "%^RESET%^ ";
        }
        result += line + "%^RESET%^\n";
    }
    write(result + "\n");
}