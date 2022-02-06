inherit STD_ROOM;

void create () {
    ::create();
    set_short("a meadow");
    set_long("A meadow filled with grasses and small flowering plants. The west side of the meadow abuts an enormous stone structure with a wide opening at its base. A framework of hexagons glimmer far overhead.");
    set_exits(([
        "enter": "/domain/Start/human/4_2_0_enter.c",
        "southeast": "/domain/Start/human/5_3_0.c",
    ]));
}