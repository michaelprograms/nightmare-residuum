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

object *verify_targets (object source, object *targets) {
    if (this_object()->query_type() == "attack") {
        if ((!targets || !targets[0])) {
            targets = ({ present_hostile(source) });
        }
        if ((!targets || !targets[0])) {
            return 0;
        }
    } else if (this_object()->query_type() == "heal" || this_object()->query_type() == "utility") {
        if ((!targets || !targets[0])) {
            targets = ({ source });
        }
        if ((!targets || !targets[0])) {
            return 0;
        }
    }
    return 0;
}