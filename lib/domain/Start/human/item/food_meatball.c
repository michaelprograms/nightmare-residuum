inherit STD_FOOD;

void create () {
    ::create();
    set_name("meatball");
    set_id(({ "meatball" }));
    set_adjective(({ "moist" }));
    set_short("a moist meatball");
    set_long("A meatball.");
    set_strength(20);
    set_value(10);
}