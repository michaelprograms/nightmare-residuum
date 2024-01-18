inherit "/std/injectable.c";

void create () {
    ::create();
    set_name("syringe");
    set_id(({ "syringe", "nanites" }));
    set_short("a syringe of nanites");
    set_long("A syringe of nanites.");
    set_type("nanites");
    set_strength(5);
    set_value(10);
}