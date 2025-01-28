nosave private string __Type;

void set_type (string type) {
    if (member_array(type, ({ "attack", "heal", "utility", })) == -1) {
        error("Bad argument 1 to ability/config->set_type");
    }
    __Type = type;
}
string query_type () {
    return __Type;
}