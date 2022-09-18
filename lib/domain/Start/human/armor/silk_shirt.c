inherit STD_ARMOR;

void create () {
    ::create();
    set_name("silk shirt");
    set_id(({ "silk shirt", "shirt" }));
    set_short("silk shirt");
    set_long("A long-sleeved shirt made of silk. The soft fabric is an off-white in color.");
    set_type("shirt");
    set_limbs(({ "torso", "left arm", "right arm" }));
    set_ac(2);
    set_autoload(1);
    set_value(20);
}