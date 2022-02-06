inherit STD_ROOM;

void create () {
    ::create();
    set_short("a hallway");
    set_long("A hallway between two tanks.");
    set_exits(([
        "west": "/domain/Start/human/tank_hallway1.c",
        "east": "/domain/Start/human/tank_hallway3.c",
    ]));
}