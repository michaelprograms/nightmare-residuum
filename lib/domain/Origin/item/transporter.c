inherit STD_ITEM;

void create () {
    ::create();
    set_id(({ "platform", }));
    set_name("platform");
    set_short("a circular platform");
    set_long("A raised platform with a step encircling it to allow one to walk on top. A circle of glowing light is illuminated in the center.");
    set_no_get(1);
}
