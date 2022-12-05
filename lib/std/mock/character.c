inherit "/std/shadow.c";

int is_character () {
    return objectp(this_object()->query_target());
}