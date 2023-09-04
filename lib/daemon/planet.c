inherit M_CLEAN;

void create () {
    set_no_clean(1);
}

mixed *query_planets () {
    object db = new("/std/database.c", ([
        "db": "/save/planet/PlanetChunks.sqlite",
    ]));
    return db->query("SELECT * FROM `Planets`");
}

mixed *query_planet (string name) {
    object db = new("/std/database.c", ([
        "db": "/save/planet/PlanetChunks.sqlite",
    ]));
    return db->query("SELECT * FROM `Chunks` WHERE name = '" + name + "'");
}