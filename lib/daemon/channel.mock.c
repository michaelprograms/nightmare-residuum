inherit "/std/shadow.c";

object *query_listeners (string channel) {
    return ({ previous_object(-1)[<1] });
}