private int __Busy = 0;
private int __Disable = 0;
private int __Immobile = 0;
private string __Posture;

// Disable prevents actions, combat hits, and movement until expired
void set_disable (int value) {
    if (value > -1) {
        __Disable = value;
    }
}
int query_disable () {
    return __Disable;
}

// Busy prevents actions until expired
void set_busy (int value) {
    if (value > -1) {
        __Busy = value;
    }
}
int query_busy () {
    return __Busy;
}

// Immobile prevents all commands until expired
void set_immobile (int value) {
    if (value > -1) {
        __Immobile = value;
    }
}
int query_immobile () {
    return __Immobile;
}

void set_posture (string p) {
    if (
        !stringp(p) &&
        !sizeof(p) &&
        member_array(p, ({ "flying", "laying", "meditating", "sitting", "standing" })) == -1
    ) {
        error("Bad argument 1 to status->set_posture");
    }
    __Posture = p;
}
string query_posture () {
    if (!__Posture) {
        __Posture = "standing";
    }
    return __Posture;
}

/* ----- applies ----- */

void heart_beat () {
    if (__Busy > 0) {
        __Busy --;
    }
    if (__Disable > 0) {
        __Disable --;
    }
    if (__Immobile > 0) {
        __Immobile --;
        if (!__Immobile) {
            message("status", "You are able to take actions again.", this_object());
        }
    }
    if (__Posture == "sitting") {
        this_object()->heal(1);
    } else if (__Posture == "laying") {
        this_object()->heal(2);
    } else if (__Posture == "meditating") {
        this_object()->heal(2);
        this_object()->add_mp(2+random(2));
    } else if (__Posture == "flying") {
        this_object()->add_sp(-(1+random(2)));
    }
}