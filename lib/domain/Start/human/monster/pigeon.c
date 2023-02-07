inherit STD_NPC;

void create () {
    ::create();
    set_gender(element_of(({ "male", "female" })));
    set_id(({ "pigeon" }));
    set_adjective(({ "wild" }));
    set_name("wild pigeon");
    set_short("wild pigeon");
    set_long("A wild pigeon, or rock dove, waddling around the street and looking for crumbs to eat.");
    set_level(4);
    set_species("pigeon");
}