private int __StatusBusy = 0;
private int __StatusDisable = 0;

// Disable prevents actions, combat hits, and movement until expired
// Busy prevents actions until expired

void set_disable (int value) {
    if (value > -1) {
        __StatusDisable = value;
    }
}
int query_disable () { return __StatusDisable; }

void set_busy (int value) {
    if (value > -1) {
        __StatusBusy = value;
    }
}
int query_busy () { return __StatusBusy; }

void heart_beat () {
    if (__StatusBusy > 0) __StatusBusy --;
    if (__StatusDisable > 0) __StatusDisable --;

    // if(!disable) continue_attack();
}

