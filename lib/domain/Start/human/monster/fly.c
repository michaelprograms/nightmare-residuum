inherit STD_MONSTER;

void create () {
    ::create();
    set_gender(element_of(({ "male", "female" })));
    set_id(({ "fly" }));
    set_adjective(({ "tiny" }));
    set_name("tiny fly");
    set_short("tiny fly");
    set_long("A fly buzzing around noisily.");
    set_level(2);
    set_species("fly");
}