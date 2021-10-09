#define CLEAN_NEVER     0
#define CLEAN_LATER     1

nosave private int __NoClean = 0;

void set_no_clean (int value) { __NoClean = value; }
int query_no_clean () { return __NoClean; }

int clean_up () {
    // @TODO check driver origin
    if (origin() == "driver" || environment() || __NoClean) {
        return CLEAN_NEVER;
    }
    if (sizeof(children(base_name(this_object()))) > 1) {
        return CLEAN_LATER;
    }
    foreach (object ob in deep_inventory(this_object())) {
        if (ob->query_user() && userp(ob->query_user())) {
            return CLEAN_LATER;
        }
    }
    return this_object()->handle_remove();
}