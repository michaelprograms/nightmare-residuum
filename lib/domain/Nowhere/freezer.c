inherit STD_ROOM;

void create () {
    ::create();
    set_short("a freezer");
    set_long("A frozen confinement.");
    set_no_clean(1);
}