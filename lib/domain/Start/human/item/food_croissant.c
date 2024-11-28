inherit STD_FOOD;

void create () {
    ::create();
    set_name("croissant");
    set_id(({ "croissant" }));
    set_adjective(({ "flakey" }));
    set_short("a flakey croissant");
    set_long("A croissant.");
    set_strength(10);
    set_value(5);
}