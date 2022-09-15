inherit STD_ARMOR;

void create () {
    ::create();
    set_name("hemp pants");
    set_id(({ "hemp pants", "pants" }));
    set_short("hemp pants");
    set_long("A pair of hemp pants. The thick fabric is a light tannish brown.");
    set_type("pants");
    set_limbs(({ "left leg", "right leg" }));
    set_ac(2);
    set_value(50);
    set_autoload(1);
}