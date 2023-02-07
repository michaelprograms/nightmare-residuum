inherit STD_NPC;

void create () {
    ::create();
    set_gender(element_of(({ "male", "female" })));
    set_id(({ "hamster" }));
    set_adjective(({ "small" }));
    set_name("small hamster");
    set_short("small hamster");
    set_long("A hamster foraging through the grass.");
    set_level(1);
    set_species("hamster");
}