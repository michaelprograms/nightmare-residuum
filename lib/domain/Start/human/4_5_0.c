inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street travelling north and south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "east": "/domain/Start/human/5_5_0.c",
        "west": "/domain/Start/human/3_5_0.c",
    ]));
}