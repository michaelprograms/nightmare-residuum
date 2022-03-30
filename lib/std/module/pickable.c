nosave private int __Picks, __MaxPicks;
nosave private string __Item, __Message;

void reset_pickable () {
    if (!environment()) return;

    if (__Picks < __MaxPicks) {
        __Picks ++;
        message("action", __Message + "\n", environment());
    }
}

void set_pickable (string item, int max, string message) {
    if (!stringp(item)) error("Bad argument 1 to pickable->set_pickable");
    if (!intp(max)) error("Bad argument 2 to pickable->set_pickable");
    if (!stringp(message)) error("Bad argument 3 to pickable->set_pickable");

    __Item = item;
    __MaxPicks = __Picks = max;
    __Message = message;
}

string query_pick_item () { return __Item; }
string query_pick_message () { return __Message; }
int query_max_picks () { return __MaxPicks; }
int query_picks () { return __Picks; }

int handle_pick () {
    if (__Picks > 0) {
        __Picks --;
        return 1;
    } else {
        return 0;
    }
}

mixed direct_pick_obj (object ob, string str) {
    return environment() == environment(ob);
}