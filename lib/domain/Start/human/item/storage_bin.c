inherit STD_STORAGE;

void create () {
    ::create();
    set_id(({ "bin", "storage bin", "storage", }));
    set_name("bin");
    set_short("a storage bin");
    set_long("A bin that can store unwanted items.");
    set_no_get(1);
}