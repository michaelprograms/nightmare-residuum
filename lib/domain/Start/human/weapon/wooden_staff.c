inherit STD_WEAPON;

void create () {
    ::create();
    set_name("wooden staff");
    set_id(({ "wooden staff", "staff" }));
    set_short("a wooden staff");
    set_long("A flimsy wooden staff.");
    set_type("blunt");
    set_value(40);
}