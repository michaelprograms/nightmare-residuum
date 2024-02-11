inherit STD_INJECTABLE;

void create () {
    ::create();
    set_name("syringe");
    set_id(({ "syringe", "nanites" }));
    set_adjective(({ "damaging" }));
    set_short("a syringe of damaging nanites");
    set_long("A syringe of damaging nanites.");
    set_type("damaging nanites");
    set_strength(50);
    set_value(50);
}