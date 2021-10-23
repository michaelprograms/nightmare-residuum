inherit STD_ROOM;

void create() {
    ::create();
    set_short("a workroom");
    set_long("A blank canvas of a workroom.");
    set_exits(([
        "void": "/domain/Nowhere/void.c",
        "freezer": "/domain/Nowhere/freezer.",
        "planet": "/domain/Planet/virtual/room/terrain/Terra/20.20",
    ]));
}