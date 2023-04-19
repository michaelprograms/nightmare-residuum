private mapping __Variables = ([]);
nosave private mapping __VariableHooks = ([]);

mixed query_variable (string v) {
    return __Variables[v];
}
varargs mixed set_variable (string v, mixed value, int set_if_undefined) {
    int changed = 0;
    if (undefinedp(set_if_undefined) || (set_if_undefined && !__Variables[v])) {
        changed = (__Variables[v] != value);
        __Variables[v] = value;
        if (changed) {
            this_object()->save_data();
            if (__VariableHooks[v]) {
                evaluate(__VariableHooks[v], value);
            }
        }
    }
    return __Variables[v];
}
int unset_variable (string v) {
    map_delete(__Variables, v);
    this_object()->save_data();
    return !__Variables[v];
}

void set_variable_hook (string v, function fn) {
    __VariableHooks[v] = fn;
}

void create () {
    if (!__Variables) {
        __Variables = ([]);
    }
    if (!__VariableHooks) {
        __VariableHooks = ([]);
    }
}