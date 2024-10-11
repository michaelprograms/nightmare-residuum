inherit "/std/shadow.c";

private int __Destructed = 0;

int is_item () {
    return objectp(query_shadow());
}
int handle_remove () {
    __Destructed = 1;
    return !__Destructed;
}

int query_destructed () {
    return __Destructed;
}