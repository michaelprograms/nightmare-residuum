inherit STD_ARMOR;

void create () {
    ::create();
    set_name("cloth pants");
    set_id(({ "cloth pants", "pants" }));
    set_short("cloth pants");
    set_long("A pair of flimsy cloth pants.");
    set_type("pants");
    set_limbs(({ "left leg", "right leg" }));
    set_ac(1);
    set_value(30);
}