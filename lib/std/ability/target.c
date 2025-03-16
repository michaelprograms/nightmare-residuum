// @this_object /std/ability.c

nosave private int __NumTargets = 1;

void set_targets (int n) {
    if (undefinedp(n) || !intp(n) || n < 1) {
        error("Bad argument 1 to ability->set_targets");
    }
    __NumTargets = n;
}
int query_targets () {
    return __NumTargets;
}