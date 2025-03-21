inherit STD_ITEM;

int is_resource () {
    return 1;
}

void create () {
    ::create();
    set_name("resource");
    set_id(({ "resource", }));
    set_short("a resource");
    set_long("A resource.");
}