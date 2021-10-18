inherit "/std/room.virtual.c";

void create() {
    ::create();
    // set_layout(
    //     "   x   \n"
    //     "  xxx  \n"
    //     " xxxxx \n"
    //     "xxxxxxx\n"
    //     " xxxxx \n"
    //     "  xxx  \n"
    //     "   x   \n"
    // );
    set_layout_file("/domain/Planet/virtual/planet.config");
    set_inheritable("/domain/Planet/virtual/base/terrain.c");
}