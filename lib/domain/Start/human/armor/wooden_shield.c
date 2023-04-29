inherit STD_ARMOR;

void create () {
    ::create();
    set_name("wooden shield");
    set_id(({ "wooden shield", "shield" }));
    set_short("wooden shield");
    set_long("A shield made of wooden.");
    set_type("shield");
    set_limbs(({ "left arm", "left hand" }));
    set_ac(2);
    set_autoload(1);
    set_value(20);
}