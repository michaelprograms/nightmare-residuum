inherit STD_ITEM;

nosave private int __Picks = 0, __MaxPicks;
nosave private string __Pickable = "";

void create () {
    ::create();

}

void reset () {
    ::reset();
    message("action", ctime()+" tree_plum " + identify(this_object())+" reset.\n", users());
    if (!environment()) return;

    if (__Picks < __MaxPicks) {
        __Picks ++;
        message("action", "A " + __Pickable->query_name() + " ripens on the tree.\n", environment());
    }
}

void set_pickable (string str) {
    if (!stringp(str)) error("Bad argument 1 to tree_pickable->set_pickable");
    __Pickable = str;
}
string query_pickable () { return __Pickable; }

void set_max_picks (int i) {
    if (!intp(i) || i < 0) error("Bad argument 1 to tree_pickable->set_max_picks");
    __MaxPicks = i;
}
int query_max_picks () { return __MaxPicks; }
int query_picks () { return __Picks; }

void handle_pick () {
    __Picks --;
}

mixed direct_pick_obj () {
    return environment() == environment(previous_object());
}