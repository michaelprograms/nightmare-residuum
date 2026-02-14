nosave private mapping __Reset, __Objects;
nosave private int __Resets;

private void initialize_reset () {
    if (!mapp(__Reset)) {
        __Reset   = ([ ]);
    }
    if (!mapp(__Objects)) {
        __Objects = ([ ]);
    }
}

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
    string name, key;
    object ob, *tracked;
    mixed val;

    initialize_reset();

    __Resets ++;
    if (!sizeof(__Reset)) {
        return;
    }

    // remove destructed tracked objects
    foreach (key, tracked in __Objects) {
        tracked -= ({ 0 });

        if (!sizeof(tracked)) {
            map_delete(__Objects, key);
        } else {
            __Objects[key] = tracked;
        }
    }

    // count objects in this container
    foreach (ob in all_inventory()) {
        name = base_name(ob);
        if (undefinedp(__Reset[name])) {
            continue;
        }
        if (arrayp(__Objects[name]) && sizeof(__Objects[name])) {
            continue;
        }
        counts[name] ++;
    }
    foreach (key, val in __Reset) {
        if (mapp(val)) {
            if (functionp(val["number"])) {
                num = evaluate(val["number"], this_object(), key);
            } else if (intp(val["number"])) {
                num = val["number"];
            } else {
                continue;
            }
        } else if (intp(val)) {
            num = val;
        } else {
            continue;
        }

        tracked = __Objects[key] || ({ });
        count = sizeof(tracked);
        if (!count && counts[key]) {
            count = counts[key];
        }

        for (; count < num; count ++) {
            if (!(ob = new(key))) {
                continue;
            }
            if (mapp(val) && functionp(val["setup"])) {
                evaluate(val["setup"], ob);
            }
            // prevent objects from leaking if can't move
            if (!ob->handle_move(this_object())) {
                destruct(ob);
                continue;
            }
            // track wandering objects
            if (ob->query_wander()) {
                if (!arrayp(__Objects[key])) {
                    __Objects[key] = ({});
                }
                __Objects[key] += ({ ob });
            }
            ob->reset();
        }
    }
}

void create () {
    initialize_reset();
    __Resets = 0;
}

void reset () {
    handle_reset();
}

void set_reset_data (mapping data) {
    __Reset = ([ ]);
    foreach (string key, mixed val in data) {
        if (!sizeof(key) || (!mapp(val) && !intp(val))) {
            error("Bad reset data to reset->set_reset_data");
        }
        if (key[<2..] == ".c") {
            key = key[0..<3];
        }
        __Reset[key] = val;
    }
}

void set_reset (mapping data) {
    set_reset_data(data);
    handle_reset();
}