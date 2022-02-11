inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village crossroads");
    set_long("Amongst a village center where two central paths form a crossroads. A plaza of stone pavers blends the intersection into a public square. Brick dwellings covered in dark ivy form a cozy enclosure around the outer circumference of the plaza. Birch trees branch over the perimeter between the dweelings and the plaza. A framework of hexagons glimmer far overhead.");
    set_looks(([
        ({ "center", "plaza", "square" }): "The central meeting point of the village, where most of the locals pass by during their day.",
        "dwellings": "Squat brick buildings of two floors in height. Modest windows with wooden shutters adorn their front.",
        ({ "birch", "trees" }): "Birch trees of various ages branch over the plaza's perimeter.",
        "hexagons": "Far above, the hexagons form a geodesic dome.",
    ]));
    set_exits(([
        "north": "/domain/Start/human/5_4_0.c",
        "east": "/domain/Start/human/6_5_0.c",
        "south": "/domain/Start/human/5_6_0.c",
        "west": "/domain/Start/human/4_5_0.c",
    ]));
    set_reset(([
        "/domain/Start/human/item/obelisk.c": 1,
        "/domain/Start/human/item/plum_tree.c": 1,
        "/domain/Start/human/item/bench.c": 2,
        "/domain/Start/human/npc/clone.c": 5,
    ]));
}