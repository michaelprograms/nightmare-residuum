inherit "/std/object/description.c";
inherit "/std/object/id.c";
inherit M_MOVE;

/* ----- applies ----- */

void create () {
    id::create();
}

int id (string id) {
    return member_array(id || "", query_id()) > -1;
}

/* ----- parser ----- */

int is_living () {
    return 0;
}