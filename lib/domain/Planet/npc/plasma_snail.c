inherit STD_NPC;

string *colors = ({
    "%^BLUE%^", "%^BLUE%^", "%^BLUE%^", "%^I_BLUE%^",
    "%^WHITE%^", "%^I_WHITE%^", "%^CYAN%^", "%^I_CYAN%^",
    "%^GREEN%^", "%^I_GREEN%^"
});

void create() {
    string plasmaSnail = "";
    foreach (string s in ({"p","l","a","s","m","a"," ","s","n","a","i","l"})) {
        plasmaSnail += element_of(colors) + s;
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