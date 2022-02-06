inherit STD_ROOM;

void create () {
    ::create();
    set_short("a hallway junction");
    set_long("A hallway junction in front of a tank. A number 2 is etched onto the wall. One hallway leads between the three tanks while the other splits away.");
    set_exits(([
        "west": "/domain/Start/human/tank_hallway1.c",
        "east": "/domain/Start/human/tank_hallway3.c",
        "north": "/domain/Start/human/tank_exit.c",
    ]));
}