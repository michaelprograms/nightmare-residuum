inherit STD_MONSTER;

void create () {
    ::create();
    set_gender("female");
    set_id(({ "ant" }));
    set_adjective(({ "curious" }));
    set_name("curious ant");
    set_short("curious ant");
    set_long("A ant curiously walking about.");
    set_level(2);
    set_species("ant");
}