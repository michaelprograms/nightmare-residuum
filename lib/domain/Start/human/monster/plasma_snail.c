inherit STD_NPC;

#define PLASMA_COLORS ({ "%^BLUE%^", "%^BLUE%^", "%^BLUE%^", "%^WHITE%^", "%^CYAN%^", "%^GREEN%^" })

void create() {
    string plasmaSnail = "%^RESET%^BOLD%^";
    foreach (string s in ({"p","l","a","s","m","a"," ","s","n","a","i","l"})) {
        plasmaSnail += element_of(PLASMA_COLORS) + s;
    }
    plasmaSnail += "%^DEFAULT%^";
    ::create();
    set_id(({ "snail", "plasma snail" }));
    set_name("plasma snail");
    set_short("a " + plasmaSnail);
    set_long("A snail swirling with a colorful array of plasma.");
    set_level(1);
    set_species("snail");
    set_ability_list(({ "zap" }));
    set_ability_chance(50);
    set_aggressive(20);
}