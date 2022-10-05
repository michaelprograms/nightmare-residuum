inherit STD_WEAPON;

void create () {
    ::create();
    set_name("iron sword");
    set_id(({ "iron sword", "sword" }));
    set_short("an iron sword");
    set_long("A flimsy iron sword but it works somewhat.");
    set_type("blade");
    set_wc(2);
    set_value(75);
}