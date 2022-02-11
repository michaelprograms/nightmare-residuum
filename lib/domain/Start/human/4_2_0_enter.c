inherit STD_ROOM;

void create () {
    ::create();
    set_short("a large stone enclosure");
    set_long("The interier cavity of a structure made of stone. A wide opening in the eastern wall leads out to a grassy meadow. Several standing lamps are scattered around the cavern, casting a bright white light.");
    set_looks(([
        ({ "structure", "cavity" }): "Inside of a stone structure. There is an opening in eastern wall.",
        "wall": "There is a wide opening through the wall out to a grassy meadow.",
        "lamps": "The standing lamps brighten the area substantially.",
        "device": "It hangs weightlessly in the air, projecting a cone of %^CYAN%^BOLD%^light%^RESET%^ down upon the body.",
    ]));
    set_exits(([
        "out east": "/domain/Start/human/4_2_0.c",
    ]));
    set_reset(([
        "/domain/Start/human/item/stasis_body.c": 1,
        "/domain/Start/human/npc/greeter.c": 1,
    ]));
}