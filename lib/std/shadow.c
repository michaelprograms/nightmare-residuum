nosave private object target;

int start_shadow (object t) {
    target = t;
    return !!shadow(target);
}
int stop_shadow () {
    if (remove_shadow(target)) {
        target = 0;
        return 1;
    } else {
        return 0;
    }
}
object query_shadow () {
    return target;
}