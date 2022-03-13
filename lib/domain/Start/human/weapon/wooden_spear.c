inherit STD_WEAPON;

void create () {
    ::create();
    set_name("wooden spear");
    set_id(({ "wooden spear", "spear" }));
    set_short("a wooden spear");
    set_long("A flimsy wooden spear.");
    set_type("projectile");
}