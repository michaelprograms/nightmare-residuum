nosave private object __LastEnv = 0;

int handle_move (mixed dest) {
    object destOb;
    int x;

    if (!this_object()) return 0;
    if (environment()) {
        x = environment()->can_release(this_object());
        if (!x /* && !archp(this_object()) */) return 0; // @TODO archp? assistp?
    }
    if (stringp(dest)) {
        if (!(destOb = find_object(dest))) {
            if (catch(load_object(dest)) ) {
                return 0;
            }
            destOb = find_object(dest);
        }
    } else {
        destOb = dest;
    }
    if (!destOb || destOb == this_object()) return 0;
    // if (living(this_object()) && living(destOb)) return 0; // @TODO
    if (!destOb->can_receive(this_object())) return 0;
    if (__LastEnv = environment()) {
        environment()->handle_release(this_object());
    }
    move_object(destOb);
    environment()->handle_receive(this_object());
    return (__LastEnv != environment());
}