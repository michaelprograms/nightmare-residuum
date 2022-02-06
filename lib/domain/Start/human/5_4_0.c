inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street travelling north and south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer far overhad.");
    set_exits(([
        "north": "/domain/Start/human/5_3_0.c",
        "south": "/domain/Start/human/5_5_0.c",
    ]));
}