inherit STD_WEAPON;

void create () {
    ::create();
    set_name("wooden sword");
    set_id(({ "wooden sword", "sword" }));
    set_short("a wooden sword");
    set_long("A wooden sword used for training.");
    set_type("blade");
    set_wc(1);
    set_value(75);
}