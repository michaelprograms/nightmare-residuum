private string __LastEnvironment;

string query_last_location () {
    if (!__LastEnvironment) return "/domain/Nowhere/room/void.c";
    return __LastEnvironment;
}
void set_last_location (string location) {
    if (!stringp(location) || !strlen(location)) error("Bad argument 1 to location->set_last_location");
    __LastEnvironment = location;
}