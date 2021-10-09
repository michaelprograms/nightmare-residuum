int maxx, maxy;

object compile_object (string arg) {
    object roomob;
    string tmp1, tmp2;
    int x, y;

    if(sscanf(arg, "%s/room/%s", tmp1, tmp2) == 2) {
        if (sscanf(tmp2, "planet-%d,%d", x, y) == 2) {
            roomob = clone_object("/domain/Planet/virtual/planet_room.c");

            if (x > 0) {
                roomob->add_exit("west", "/domain/Planet/virtual/room/planet-"+(x-1)+","+(y));
                if (y > 0) {
                    roomob->add_exit("northwest", "/domain/Planet/virtual/room/planet-"+(x-1)+","+(y-1));
                }
                if (y < maxy) {
                    roomob->add_exit("southwest", "/domain/Planet/virtual/room/planet-"+(x-1)+","+(y+1));
                }
            }
            if (x < maxx) {
                roomob->add_exit("east", "/domain/Planet/virtual/room/planet-"+(x+1)+","+(y));
                if (y > 0) {
                    roomob->add_exit("northeast", "/domain/Planet/virtual/room/planet-"+(x+1)+","+(y-1));
                }
                if (y < maxy) {
                    roomob->add_exit("southeast", "/domain/Planet/virtual/room/planet-"+(x+1)+","+(y+1));
                }
            }
            if (y > 0) {
                roomob->add_exit("north", "/domain/Planet/virtual/room/planet-"+(x)+","+(y-1));
            }
            if (y < maxy) {
                roomob->add_exit("south", "/domain/Planet/virtual/room/planet-"+(x)+","+(y+1));
            }
        }
    }
    return roomob;
}

void create() {
    maxx = 59;
    maxy = 59;
}
