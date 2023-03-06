inherit STD_NPC;

void create () {
    ::create();
    set_gender(element_of(({ "male", "female" })));
    set_id(({ "sparrow" }));
    set_adjective(({ "skittish" }));
    set_name("skittish sparrow");
    set_short("skittish sparrow");
    set_long("A skittish sparrow quickly flying around the street and looking for food.");
    set_level(3);
    set_species("sparrow");
}