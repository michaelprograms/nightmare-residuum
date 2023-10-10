inherit STD_ITEM;
inherit M_LEVELABLE;

void create () {
    ::create();
    set_name("resource node");
    set_id(({"resource", "resource node", "node", "resource_node", }));
    set_short("a resource node");
    set_long("A resource node.");
    set_no_get(1);
}