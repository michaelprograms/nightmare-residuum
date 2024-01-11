inherit STD_NPC;

void create () {
    ::create();
    set_gender(element_of(({ "male", "female" })));
    set_id(({ "cat" }));
    set_adjective(({ "feral" }));
    set_name("feral cat");
    set_short("feral cat");
    set_long("A feral cat watching you suspiciously.");
    set_level(3);
    set_species("cat");
}