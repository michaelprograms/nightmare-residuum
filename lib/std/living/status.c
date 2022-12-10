private int __Busy = 0, __Disable = 0;
private string __Posture;

// Disable prevents actions, combat hits, and movement until expired
void set_disable (int value) {
    if (value > -1) {
        __Disable = value;
    }
}
int query_disable () { return __Disable; }

// Busy prevents actions until expired
void set_busy (int value) {
    if (value > -1) {
        __Busy = value;
    }
}
int query_busy () { return __Busy; }

void set_posture (string p) {
    if (!stringp(p) && !sizeof(p)) error("Bad argument 1 to status->set_posture");
    if (member_array(p, ({ "sit", "lay", "rest", "sleep", })) > -1) error("Bad argument 1 to status->set_posture");
    __Posture = p;
}


/* ----- applies ----- */

void heart_beat () {
    if (__Busy > 0) __Busy --;
    if (__Disable > 0) __Disable --;
    if (__Posture = "rest") this_object()->heal(1);
    if (__Posture = "sleep") this_object()->heal(2);
}