inherit STD_NPC;

void create () {
    ::create();
    set_gender(element_of(({ "male", "female" })));
    set_id(({ "dog" }));
    set_adjective(({ "wild" }));
    set_name("wild dog");
    set_short("wild dog");
    set_long("A wild dog scavenging through the the area.");
    set_level(3);
    set_species("dog");
}