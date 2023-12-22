inherit STD_NPC;

void create () {
    ::create();
    set_gender(element_of(({ "male", "female" })));
    set_id(({ "crow" }));
    set_adjective(({ "watchful" }));
    set_name("watchful crow");
    set_short("watchful crow");
    set_long("A watchful crow surveying the area with an everpresent gaze.");
    set_level(3);
    set_species("crow");
}