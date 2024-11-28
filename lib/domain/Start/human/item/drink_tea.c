inherit STD_DRINK;

void create () {
    ::create();
    set_name("tea");
    set_id(({ "tea" }));
    set_adjective(({ "iced" }));
    set_short("a cup of iced tea");
    set_long("A cup of iced tea.");
    set_strength(10);
    set_value(5);
}