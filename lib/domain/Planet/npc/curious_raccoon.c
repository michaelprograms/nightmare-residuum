inherit STD_NPC;

void create () {
    ::create();
    set_gender(element_of(({ "male", "female" })));
    set_id(({ "raccoon" }));
    set_adjective(({ "curious" }));
    set_name("curious raccoon");
    set_short("curious raccoon");
    set_long("A curious raccoon scavenging for food.");
    set_level(1);
    set_species("raccoon");
    set_ability_list(({ "bite", "scratch", }));
}