inherit STD_NPC;

void create () {
    ::create();
    set_gender(element_of(({ "male", "female" })));
    set_id(({ "squirrel" }));
    set_adjective(({ "nosey" }));
    set_name("nosey squirrel");
    set_short("nosey squirrel");
    set_long("A squirrel watching your every move.");
    set_level(4);
    set_species("squirrel");
}