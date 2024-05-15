nosave private object target;

int start_shadow (object t) {
    target = t;
    return !!shadow(target);
}
int stop_shadow () {
    object t = target;
    target = 0;
    return t && remove_shadow(t);
}
object query_shadow () {
    return target;
}