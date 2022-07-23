inherit STD_MONSTER;

#define PLASMA_COLORS ({"%^BLUE%^BOLD%^", "%^BLUE%^", "%^BLUE%^BOLD%^", "%^CYAN%^BOLD%^", "%^CYAN%^", "%^GREEN%^BOLD%^", })

void create() {
    string plasma = "";
    ::create();
    foreach (string s in ({ "p", "l", "a", "s", "m", "a" })) {
        plasma += element_of(PLASMA_COLORS) + s;
    }
    plasma = "%^RESET" + plasma + "%^DEFAULT%^";
    set_id(({ "snail", "plasma snail" }));
    set_name("plasma snail");
    set_short("a " + plasma + " snail");
    set_long("A snail swirling with a colorful array of " + plasma + ".");
    set_level(3);
    set_species("snail");
}