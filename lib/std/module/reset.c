nosave private mapping __Reset, __Objects;
nosave private int __Resets;

mapping query_reset () {
    return __Reset;
}
mapping query_objects () {
    return __Objects;
}
int query_resets () {
    return __Resets;
}

void handle_reset () {
    mapping counts = ([ ]);
    int count, num;
    string name;
    object ob;

    // remove destructed tracked objects
    __Objects = filter_mapping(__Objects, (: !!$2 :));

    // count objects in this container
    foreach (ob in all_inventory()) {
        name = base_name(ob) + ".c";
        if (!__Reset[name]) {
            continue;
        }
        if (sizeof(__Objects[name]) > 0) {
            continue;
        }
        if (!counts[name]) {
            counts[name] = 0;
        }
        counts[name] += 1;
    }
    foreach (string key, mixed val in __Reset) {
        if (mapp(val)) {
            if (functionp(val["number"])) {
                num = evaluate(val["number"]);
            } else if (intp(val["number"])) {
                num = val["number"];
            } else {
                continue;
            }
        } else if (intp(val)) {
            num = val;
        }

        count = 0;
        if (counts[key] && (count = counts[key]) >= num) {
            continue;
        }
        for (; count < num; count ++) {
            if (__Objects[key + ":" + count]) {
                continue;
            }

            ob = new(key);
            if (mapp(val) && functionp(val["setup"])) {
                evaluate(val["setup"], ob);
            }

            // track wandering objects
            if (ob->query_wander()) {
                __Objects[key + ":" + count] = ob;
            }

            // prevent objects from leaking if can't move
            if (!ob->handle_move(this_object())) {
                destruct(ob);
            } else {
                ob->reset();
            }
        }
    }
    __Resets ++;
}

void create () {
    __Reset = ([ ]);
    __Objects = ([ ]);
    __Resets = 0;
}

void reset () {
    handle_reset();
}

void set_reset_data (mapping data) {
    __Reset = ([ ]);
    foreach (string key, mixed val in data) {
        if (!sizeof(key) || !mapp(val) && !intp(val)) {
            error("Bad reset data to reset->set_reset_data");
        }
        __Reset[key] = val;
    }
}

void set_reset (mapping data) {
    set_reset_data(data);
    reset();
}