inherit STD_NPC;
inherit M_SAVE;

private int __Following;

nosave private object __Owner;

void set_owner (object owner) {
    if (!objectp(owner)) error("Bad argument 1 to follower->set_owner");

    __Owner = owner;
}
object query_owner () {
    return __Owner;
}
string query_owner_name () {
    if (__Owner) {
        return __Owner->query_key_name();
    } else {
        return 0;
    }
}

void set_following (int i) {
    if (!intp(i)) error("Bad argument 1 to follower->set_following");
    __Following = 1;
}
int query_following () {
    return __Following;
}

/* ----- applies ----- */

// void heart_beat () {
//     object env;
//     ::heart_beat();


//     // if (__Owner && )
// }