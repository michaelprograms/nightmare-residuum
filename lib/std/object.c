inherit "/std/object/description.c";
inherit "/std/object/id.c";
inherit M_MOVE;

nosave private string __SpawnedBy;

/* ----- applies ----- */

void create() {
    if (previous_object()) {
        __SpawnedBy = base_name(previous_object());
    }
    id::create();
}

/**
 * Return the base_name of the object that cloned this one. Stored as a name
 * rather than an object reference so it survives the spawning object being
 * destructed (ex: a room clean up / deep reset) and reloaded as a new object,
 * letting reset re-identify its own wandering spawns.
 *
 * @returns {string} base_name of the cloning object, or 0 if there was none
 */
string query_spawned_by() {
    return __SpawnedBy;
}

int id(string id) {
    return member_array(id || "", query_id()) > -1;
}

/* ----- parser ----- */

int is_living() {
    return 0;
}
