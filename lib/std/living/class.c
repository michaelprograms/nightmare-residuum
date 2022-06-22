private string __Class;
private string __Subclass;

void set_class (string c) {
    // @TODO security
    // @TODO class verification
    __Class = c;
    __Subclass = 0;
    this_object()->update_vitals(1);
}
string query_class () {
    return __Class || "adventurer";
}

void set_subclass (string c) {
    // @TODO security
    // @TODO subclass verification
    __Subclass = c;
}
string query_subclass() {
    return __Subclass || "none";
}