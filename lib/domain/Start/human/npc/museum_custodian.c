inherit STD_NPC;

string number = ""+random(2)+random(2)+random(2)+random(2)+random(2)+random(2)+random(2);
void create () {
    ::create();
    set_name("R-" + number);
    set_id(({ "synthetic creature", "synthetic", "creature", "r-"+number }));
    set_short("a synthetic creature");
    set_long("A creature seemingly constructed of synthetic components. It consists of a large elongated torso structure supported by four jointed legs, all encased in a scale-like exoskeleton. There are two articulated arms at each end of the structure, almost looking like a type of crustacean. The creature does seem to have a discernable head or face.");
    set_species("custodian");
}