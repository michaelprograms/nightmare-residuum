inherit STD_NPC;

void create () {
    ::create();
    set_gender(element_of(({ "male", "female" })));
    set_id(({ "vulture" }));
    set_adjective(({ "lurking" }));
    set_name("lurking vulture");
    set_short("lurking vulture");
    set_long("A lurking vulture which plans the next meal.");
    set_level(3);
    set_species("vulture");
}