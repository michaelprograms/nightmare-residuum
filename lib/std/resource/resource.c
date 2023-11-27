inherit STD_ITEM;

string __ResourceType;

string query_type () {
    return __ResourceType;
}
void set_type (string type) {
    __ResourceType = type;
}

void create () {
    ::create();
    set_name("resource");
    set_id(({ "resource", }));
    set_short("a resource");
    set_long("A resource.");
}