inherit "/std/shadow.c";

int is_character () {
    return objectp(query_shadow());
}