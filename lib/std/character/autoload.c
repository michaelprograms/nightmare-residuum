private mixed *__Autoload;

/* -----  ----- */

protected void reset_autoload () {
    __Autoload = ({ });
}

mixed *query_autoload_items () {
    return __Autoload;
}

varargs void update_autoload (int exit) {
    object *inv = all_inventory(this_object());
    int i, l = sizeof(inv);
    mixed *data;

    reset_autoload();
    for (i = 0; i < l; i ++) {
        if (data = inv[i]->query_autoload(this_object())) {
            __Autoload += ({ data });
            if (exit) {
                inv[i]->handle_remove();
            }
        }
    }
}

void restore_autoload () {
    int i, l = sizeof(__Autoload);
    object ob;
    string err;

    for (i = 0; i < l; i ++) {
        if (!sizeof(__Autoload[i])) {
            continue;
        }
        if (!stringp(__Autoload[i][0])) {
            continue;
        }

        err = catch(ob = new(__Autoload[i][0]));
        if (err || !ob) {
            if (this_object()->query_immortal()) {
                write("Error in character->restore_autoload " + __Autoload[i][0] + " : " + err + "\n");
            }
            continue;
        }
        if (!ob->handle_move(this_object()) || !ob) {
            continue;
        }
        if (sizeof(__Autoload[i]) > 1) {
            err = catch (ob->restore_autoload(__Autoload[i][1..]));
            if (err && this_object()->query_immortal()) {
                write("Error in character->restore_autoload " + __Autoload[i][0] + " : " + err + "\n");
            }
        }
    }
}

/* ----- applies ----- */

void create () {
    reset_autoload();
}