inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village crossroads");
    set_long("Amongst a village center where two central paths form a crossroads. A plaza of stone pavers blends the intersection into a public square. Brick dwellings covered in dark ivy form a cozy enclosure around the outer circumference of the plaza. Birch trees branch over the perimeter between the dweelings and the plaza. A framework of hexagons glimmer far overhad.");
    set_exits(([
        "northwest": "/domain/Start/human/4_2_0.c",
        "north": "/domain/Start/human/5_2_0.c",
        "south": "/domain/Start/human/5_4_0.c",
    ]));
}