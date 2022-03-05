inherit STD_WEAPON;

void create () {
    ::create();
    set_name("iron sword");
    set_id(({ "iron sword", "sword" }));
    set_short("a iron sword");
    set_long("A flimsy iron sword.");
    set_type("blade");
}