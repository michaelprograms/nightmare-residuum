inherit STD_WEAPON;

void create () {
    ::create();
    set_name("wooden staff");
    set_id(({ "wooden staff", "staff" }));
    set_short("a wooden staff");
    set_long("A rough wooden staff just shorter than a human is tall.");
    set_type("blunt");
    set_value(40);
}