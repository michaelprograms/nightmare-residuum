inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street travelling north and south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A meadow is to the northwest. A framework of hexagons glimmer overhead.");
    set_exits(([
        "northwest": "/domain/Start/human/4_2_0.c",
        "north": "/domain/Start/human/5_2_0.c",
        "south": "/domain/Start/human/5_4_0.c",
    ]));
}