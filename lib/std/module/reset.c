nosave private mapping __Reset;
nosave private int __Resets;

mapping query_reset () {
    return __Reset;
}
int query_resets () {
    return __Resets;
}

void create () {
    __Reset = ([ ]);
    __Resets = 0;
}
void reset () {
    mapping counts = ([ ]);
    int count;
    string name;

    foreach (object ob in all_inventory()) {
        name = base_name(ob) + ".c";
        if (!__Reset[name]) continue;
        if (!counts[name]) counts[name] = 0;
        counts[name] += 1;
    }
    foreach (string key, mixed val in __Reset) {
        if (intp(val)) {
            count = 0;
            if (counts[key] && (count = counts[key]) >= val) continue;
            while (count < val) {
                object ob = clone_object(key);
                // prevent objects from leaking if can't move
                if (!ob->handle_move(this_object())) {
                    destruct(ob);
                } else {
                    ob->reset();
                    ob->setup_inventory();
                }
                count ++;
            }
        } else if (functionp(val)) {
            if (evaluate(val)) {
                object ob = clone_object(key);
                ob->handle_move(this_object());
                ob->reset();
                ob->setup_inventory();
            }
        }
    }
    __Resets ++;
}

void set_reset (mapping reset) {
    __Reset = ([ ]);
    foreach (string key, mixed val in reset) {
        __Reset[key] = val;
    }
    reset();
}