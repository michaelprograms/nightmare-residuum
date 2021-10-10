inherit "/std/room.virtual.c";

void create() {
    ::create();
    set_layout(
        " x x \n"
        "xxxxx\n"
        " x x \n"
    );
    set_inheritable("/domain/Planet/virtual/base/terrain.c");
}