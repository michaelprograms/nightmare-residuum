nosave private int __Picks = 0, __MaxPicks = 0;
nosave private string __Item, __Message;

// called by STD_ITEM->reset
void reset_pickable () {
    if (__Picks < __MaxPicks) {
        __Picks ++;
        if (environment()) {
            message("action", __Message, environment());
        }
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

mixed direct_pick_obj (mixed args...) {
    object ob;
    if (sizeof(args)) {
        ob = args[0];
        return environment() == environment(ob);
    } else return 0;
}