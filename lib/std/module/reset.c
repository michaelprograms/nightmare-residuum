nosave private mapping __Reset;
nosave private int __Resets;

mapping query_reset () {
    return __Reset;
}
int query_resets () {
    return __Resets;
}

void create () {
    __Reset = ([]);
    __Resets = 0;
}
void reset () {
    mapping counts = ([]);
    int count;
    string name;

    foreach (object ob in all_inventory(this_object())) {
        name = base_name(ob) + ".c";
        if (!counts[name]) counts[name] = 0;
        counts[name] += 1;
    }
    foreach (string key, mixed val in __Reset) {
        if (intp(val)) {
            count = 0;
            if (counts[key] && (count = counts[key]) >= val) continue;
            while (count < val) {
                clone_object(key)->handle_move(this_object());
                count ++;
            }
        } else if (functionp(val)) {
            if (evaluate(val)) {
                clone_object(key)->handle_move(this_object());
            }
        }
    }
    __Resets ++;
}

void set_reset (mapping reset) {
    __Reset = ([]);
    foreach (string key, mixed val in reset) {
        __Reset[key] = val;
    }
    reset();
}