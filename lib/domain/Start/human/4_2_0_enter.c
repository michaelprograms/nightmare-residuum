inherit STD_ROOM;

void create () {
    ::create();
    set_short("a large stone enclosure");
    set_long("The interier cavity of a structure made of stone. An opening in the side of a wall leads out to a small grassy meadow. Large dim orbs scattered along the ceiling cast a yellow light.");
    set_exits(([
        "out": "/domain/Start/human/4_2_0.c",
    ]));
}