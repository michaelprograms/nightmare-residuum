nosave private int __Protection;

int query_protection () {
    if (undefinedp(__Protection)) {
        __Protection = 0;
    }
    return __Protection;
}
int set_protection (int n) {
    __Protection = n;
    return __Protection;
}
int clear_protection () {
    int hadProtection = (__Protection > 0);

    __Protection = 0;
    if (this_object() && hadProtection) {
        while (find_call_out("remove_protection") > -1) {
            remove_call_out("remove_protection");
        }
        message("protection", "The protective shield around you fades away.", this_object());
    }
    return __Protection;
}
int add_protection (int n, int time) {
    int hadProtection = (__Protection > 0);

    __Protection += n;
    if (this_object()) {
        if (hadProtection) {
            message("protection", "A protective shield strengthens around around you.", this_object());
        } else {
            message("protection", "A protective shield forms around around you.", this_object());
        }
    }

    if (time > 0) {
        call_out("remove_protection", time, n);
    }
    return __Protection;
}
int remove_protection (int n) {
    int hadProtection = (__Protection > 0);

    __Protection -= n;
    if (this_object() && hadProtection) {
        message("protection", "The protective shield around you fades " + (__Protection > 0 ? "slightly" : "away") + ".", this_object());
    }

    return __Protection;
}

int handle_remove () {
    clear_protection();
}