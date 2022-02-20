inherit STD_FOOD;

void create () {
    ::create();
    set_name("plum");
    set_id(({ "plum" }));
    set_adjective(({ "juicy" }));
    set_short("a juicy %^RESET%^MAGENTA%^plum");
    set_long("A plum.");
    set_strength(5);
}