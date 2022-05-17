inherit STD_MONSTER;

void create () {
    ::create();
    set_gender(element_of(({ "male", "female" })));
    set_id(({ "beetle" }));
    set_adjective(({ "oblivious" }));
    set_name("oblivious beetle");
    set_short("oblivious beetle");
    set_long("A beetle obliviously walking about.");
    set_level(3);
    set_species("beetle");
}