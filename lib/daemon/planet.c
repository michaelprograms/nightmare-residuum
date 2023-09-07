inherit M_CLEAN;

#define CHUNK_SIZE 32

void create () {
    set_no_clean(1);
}

mixed *query_planets () {
    object db = new(STD_DATABASE, ([
        "db": "/save/planet/PlanetChunks.sqlite",
    ]));
    return db->query("SELECT * FROM `Planets`");
}

mixed *query_planet (string name) {
    object db = new(STD_DATABASE, ([
        "db": "/save/planet/PlanetChunks.sqlite",
    ]));
    return db->query("SELECT * FROM `Planets` WHERE `name` = '" + name + "'");
}

mixed *query_planet_chunks (string name) {
    object db = new(STD_DATABASE, ([
        "db": "/save/planet/PlanetChunks.sqlite",
    ]));
    return db->query("SELECT * FROM `Chunks` WHERE `name` = '" + name + "'");
}

mixed *query_planet_chunk (string name, int x, int y) {
    object db = new(STD_DATABASE, ([
        "db": "/save/planet/PlanetChunks.sqlite",
    ]));
    int cx, cy;
    mixed result;

    cx = x / CHUNK_SIZE;
    cy = y / CHUNK_SIZE;

    result = db->query("SELECT * FROM `Chunks` WHERE `name` = '" + name + "' AND `cx` = '" + cx + "' AND `cy` = '" + cy + "'");

    return sizeof(result) ? result[0] : 0;
}