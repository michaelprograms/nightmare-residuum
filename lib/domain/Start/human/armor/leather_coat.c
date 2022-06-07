inherit STD_ARMOR;

void create () {
    ::create();
    set_name("leather coat");
    set_id(({ "leather coat", "coat" }));
    set_short("leather coat");
    set_long("A flimsy leather coat.");
    set_type("coat");
    set_limbs(({ "torso", "left arm", "right arm" }));
    set_ac(2);
    set_bonus("endurance", 1);
    set_value(50);
}