inherit STD_NPC;

void create () {
    ::create();
    set_gender(element_of(({ "male", "female" })));
    set_id(({ "rat" }));
    set_adjective(({ "greasy" }));
    set_name("greasy rat");
    set_short("greasy rat");
    set_long("A greasy rat peering at you cautiously.");
    set_level(1);
    set_species("rat");
}