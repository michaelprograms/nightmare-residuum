inherit STD_NPC;

void create () {
    ::create();
    set_gender(element_of(({ "male", "female" })));
    set_id(({ "ant" }));
    set_adjective(({ "large" }));
    set_name("large ant");
    set_short("large ant");
    set_long("A large ant approaching you filled with curiosity.");
    set_level(1);
    set_species("ant");
    set_ability_list(({ "bite", "scratch", }));
}