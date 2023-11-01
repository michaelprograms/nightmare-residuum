inherit STD_ITEM;

string __ResourceType;

string query_type () {
    return __ResourceType;
}
void set_type (string type) {
    __ResourceType = type;
}