inherit STD_MONSTER;

void create () {
    ::create();
    set_gender("female");
    set_id(({ "bee" }));
    set_adjective(({ "bumbling" }));
    set_name("bumbling bee");
    set_short("bumbling bee");
    set_long("A bee bumbling around carelessly.");
    set_level(2);
    set_species("bee");
}