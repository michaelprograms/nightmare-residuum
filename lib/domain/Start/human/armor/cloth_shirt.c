inherit STD_ARMOR;

void create () {
    ::create();
    set_name("cloth shirt");
    set_id(({ "cloth shirt", "shirt" }));
    set_short("cloth shirt");
    set_long("A flimsy cloth shirt.");
    set_type("shirt");
    set_limbs(({ "torso", "left arm", "right arm" }));
    set_ac(1);
}