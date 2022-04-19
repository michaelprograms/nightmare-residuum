inherit STD_MONSTER;

void create () {
    ::create();
    set_gender("female");
    set_id(({ "termite" }));
    set_adjective(({ "eager" }));
    set_name("eager termite");
    set_short("eager termite");
    set_long("A termite eagerly feeling about.");
    set_level(2);
    set_species("termite");
}