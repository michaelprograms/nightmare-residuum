inherit "/std/module/description.c";
inherit "/std/module/id.c";
inherit M_MOVE;

// --- APPLIES -----------------------------------------------------------------

void create () {
    id::create();
}

// void heart_beat () {
//     /*
//     If an object has called set_heart_beat(), this function will be called
//     periodically in that object.
//     */
// }

int id (string id) {
    /*
    The  present(3)  efunction calls id() to determine if a given object is
    named by a given string.  id() should return 1 if the object wishes  to
    be known by the name in the string an Id; it should return 0 otherwise.
    */
    return member_array(id, query_id()) > -1;
}

// int move_or_destruct (object parent) {
//     // Interactive should handle this.
//     /*
//     If an object is to be destructed, prior to its destruction this apply is called on its contents. 'parent' is the object that will be destructed. If the target object of this apply does not move itself out of the object being destructed, it will be destructed as well.
//     */
// }

void reset () {

}

// void virtual_start () {
//     /*
//     After virtual object is created, driver will call this function to trigger
//     addtional initialization.
//     */
// }

// --- PARSER ------------------------------------------------------------------

int is_living () {
    return 0;
}