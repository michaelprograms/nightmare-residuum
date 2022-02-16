inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street travelling north and south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "east": "/domain/Start/human/8_5_0.c",
        "west": "/domain/Start/human/6_5_0.c",
    ]));
}