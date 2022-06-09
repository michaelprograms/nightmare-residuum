inherit STD_WEAPON;

void create () {
    ::create();
    set_name("iron sword");
    set_id(({ "iron sword", "sword" }));
    set_short("an iron sword");
    set_long("An flimsy iron sword.");
    set_type("blade");
    set_value(75);
}