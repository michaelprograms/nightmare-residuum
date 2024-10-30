private mixed *__Autoload;

/* -----  ----- */

protected void reset_autoload () {
    __Autoload = ({ });
}

mixed *query_autoload_items () {
    return __Autoload;
}

varargs void update_autoload (int exit) {
    object *inv = all_inventory();
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
        if (sizeof(__Autoload[i]) && stringp(__Autoload[i][0])) {
            err = catch(ob = new(__Autoload[i][0]));
            if (!err && ob && ob->handle_move(this_object())) {
                if (sizeof(__Autoload[i]) > 1) {
                    catch (ob->restore_autoload(__Autoload[i][1..]));
                }
            }
        }
    }
}

/* ----- applies ----- */

void create () {
    reset_autoload();
}