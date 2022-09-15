inherit STD_ARMOR;

void create () {
    ::create();
    set_name("hemp shirt");
    set_id(({ "hemp shirt", "shirt" }));
    set_short("hemp shirt");
    set_long("A long-sleeved shirt made of hemp. The thick fabric is a light tannish brown.");
    set_type("shirt");
    set_limbs(({ "torso", "left arm", "right arm" }));
    set_ac(2);
    set_autoload(1);
    set_value(20);
}