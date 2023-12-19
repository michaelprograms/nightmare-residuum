inherit STD_NPC;

void create () {
    ::create();
    set_gender(element_of(({ "male", "female" })));
    set_id(({ "rat" }));
    set_adjective(({ "rabid" }));
    set_name("rabid rat");
    set_short("rabid rat");
    set_long("A rabid rat lurching uncomfortably towards you.");
    set_level(1);
    set_species("rat");
    set_ability_list(({ "bite", "scratch", }));
}