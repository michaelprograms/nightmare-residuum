inherit STD_ITEM;

string __ResourceType;

int is_resource () {
    return 1;
}

string query_type () {
    return __ResourceType;
}
void set_type (string type) {
    __ResourceType = type;
}