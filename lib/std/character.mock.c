inherit "/std/shadow.c";

int is_character () {
    return objectp(query_shadow());
}
int is_living () {
    return objectp(query_shadow());
}