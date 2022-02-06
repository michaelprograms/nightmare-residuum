inherit STD_ROOM;

void create () {
    ::create();
    set_short("a hallway");
    set_long("A hallway in front of a tank. A number 2 is etched onto the wall.");
    set_exits(([
        "west": "/domain/Start/human/tank_hallway2.c",
    ]));
}