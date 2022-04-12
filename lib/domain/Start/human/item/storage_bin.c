inherit STD_STORAGE;

void create () {
    ::create();
    set_id(({ "bin", "donation bin", }));
    set_name("bin");
    set_short("a donation bin");
    set_long("A bin that contains donations.");
    set_no_get(1);
}