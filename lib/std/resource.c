inherit STD_ITEM;

void create () {
    ::create();
    set_name("resource node");
    set_id(({"resource", "resource node", "node" }));
    set_short("a resource node");
    set_long("A resource node.");
    set_no_get(1);
}