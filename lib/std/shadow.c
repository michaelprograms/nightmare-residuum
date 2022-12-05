nosave private object target;

int start_shadow (object t) {
    target = t;
    return !!shadow(target);
}
int stop_shadow () {
    return remove_shadow(target);
}
object query_target () {
    return target;
}