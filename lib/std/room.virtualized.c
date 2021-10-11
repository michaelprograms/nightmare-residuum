inherit STD_ROOM;

nosave private string __Server;

void set_server (string server) {
    __Server = server;
}
string query_server () {
    return __Server;
}

string *query_map () {
    if (!__Server) {
        return 0;
    } else {
        return __Server->query_map();
    }
}
