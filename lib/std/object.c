inherit "/std/module/description.c";
inherit "/std/module/id.c";
inherit M_MOVE;

/* ----- applies ----- */

void create () {
    id::create();
}

// void heart_beat () { }

int id (string id) {
    return member_array(id || "", query_id()) > -1;
}

// int move_or_destruct (object parent) { }

void reset () {

}

// void virtual_start () { }

/* ----- parser ----- */

int is_living () {
    return 0;
}