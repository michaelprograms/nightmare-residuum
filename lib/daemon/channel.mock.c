inherit "/std/shadow.c";

object *query_listeners(string _channel) {
    return ({ previous_object(-1)[<1] });
}
