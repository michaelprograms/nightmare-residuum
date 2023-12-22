inherit STD_NPC;

void create () {
    ::create();
    set_gender(element_of(({ "male", "female" })));
    set_id(({ "crow" }));
    set_adjective(({ "lurking" }));
    set_name("lurking crow");
    set_short("lurking crow");
    set_long("A lurking crow watching the area with an everpresent gaze.");
    set_level(3);
    set_species("crow");
}