nosave private int __Protection;

void clear_protection () {
    // @TODO remove timers?
    __Protection = 0;
}
void set_protection (int n) {
    __Protection = n;
}
int add_protection (int n, int time) {
    __Protection += n;

    if (n >= 0 && time > 0) {
        call_out("add_protection", time, -n, 0);
        message("protection", "A protective shield forms around around you.", this_object());
    } else {
        message("protection", "The protective shield around you fades " + (__Protection ? "slightly" : "away") + ".", this_object());
    }
    return 1;
}
int query_protection () {
    return __Protection;
}