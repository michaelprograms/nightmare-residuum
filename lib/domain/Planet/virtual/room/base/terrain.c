inherit STD_ROOM;

nosave private float __Terrain;
nosave private string *__TerrainOverride = ({ });

void add_terrain_override (string text) {
    __TerrainOverride += ({ text });
}

float query_terrain () {
    return __Terrain;
}
void set_terrain (float t) {
    __Terrain = t;

    if (__Terrain <= 0.2) {
        set_room_square_color("\e[38;5;4m");     // BLUE
        set_short("deep water");
        set_long("Surrounded by deep water.");
    } else if (__Terrain <= 0.25) {
        set_room_square_color("\e[38;5;12m");    // BOLD BLUE
        set_short("water");
        set_long("Surrounded by water.");
    } else if (__Terrain <= 0.3) {
        set_room_square_color("\e[38;5;6m");     // CYAN
        set_short("shallow water");
        set_long("Wading through shallow water.");
    } else if (__Terrain <= 0.4) {
        set_room_square_color("\e[38;5;14m");    // BOLD CYAN
        set_short("puddles");
        set_long("Stomping through puddles.");
    } else if (__Terrain <= 0.5) {
        set_room_square_color("\e[38;5;11m");    // YELLOW
        set_short("arid sands");
        set_long("Surrounded by arid sands.");
    } else if (__Terrain <= 0.6) {
        set_room_square_color("\e[38;5;10m");    // BOLD GREEN
        set_short("grassy fields");
        set_long("Surrounded by grassy fields.");
    } else if (__Terrain <= 0.7) {
        set_room_square_color("\e[38;5;2m");     // GREEN
        set_short("forests");
        set_long("Surrounded by forests.");
    } else if (__Terrain <= 0.8) {
        set_room_square_color("\e[38;5;3m");     // ORANGE
        set_short("mountainous terrain");
        set_long("Surrounded by mountainous terrain.");
    } else if (__Terrain <= 0.9) {
        set_room_square_color("\e[38;5;7m");     // WHITE
        set_short("snow dusted hills");
        set_long("Surrounded by snow dusted hills.");
    } else if (__Terrain <= 1.0) {
        set_room_square_color("\e[38;5;15m");    // BOLD WHITE
        set_short("snow covered hills");
        set_long("Surrounded by snow covered hills.");
    } else {
        set_room_square_color("\e[38;5;9m");
    }

    if (sizeof(__TerrainOverride)) {
        set_long(query_long() + " " + implode(__TerrainOverride, " "));
    }
}

int is_virtual_room () { return 1; }

void create() {
    ::create();
    set_short("a terrain somewhere");
    set_long("The terrain of a planet.");
}
