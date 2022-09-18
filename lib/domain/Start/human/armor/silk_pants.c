inherit STD_ARMOR;

void create () {
    ::create();
    set_name("silk pants");
    set_id(({ "silk pants", "pants" }));
    set_short("silk pants");
    set_long("A pair of silk pants. The soft fabric is an off-white color.");
    set_type("pants");
    set_limbs(({ "left leg", "right leg" }));
    set_ac(2);
    set_value(50);
    set_autoload(1);
}