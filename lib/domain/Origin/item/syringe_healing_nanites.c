inherit STD_INJECTABLE;

void create () {
    ::create();
    set_name("syringe");
    set_id(({ "syringe", "nanites" }));
    set_adjective(({ "healing" }));
    set_short("a syringe of healing nanites");
    set_long("A syringe of healing nanites.");
    set_type("healing nanites");
    set_strength(50);
    set_value(25);
}