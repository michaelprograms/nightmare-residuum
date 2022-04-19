inherit STD_MONSTER;

void create () {
    ::create();
    set_gender("female");
    set_id(({ "wasp" }));
    set_adjective(({ "aggressive" }));
    set_name("aggressive wasp");
    set_short("aggressive wasp");
    set_long("A wasp aggressively zipping about.");
    set_level(2);
    set_species("wasp");
}