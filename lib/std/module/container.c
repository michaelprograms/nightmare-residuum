/**
 * Check if this object can receive the specified object.
 *
 * @param {STD_OBJECT} ob the object to receive
 * @returns 0 or 1 for success
 */
int can_receive (object ob) {
    return member_array(ob, all_inventory()) == -1;
}
/**
 * Check if this object can release the specified object.
 *
 * @param {STD_OBJECT} ob the object to release
 * @returns 0 or 1 for success
 */
int can_release (object ob) {
    return member_array(ob, all_inventory()) > -1;
}

int handle_receive (object ob) {
    return !!ob;
}
int handle_release (object ob) {
    return !!ob;
}

int inventory_visible () {
    return 1;
}
int inventory_accessible () {
    return 1;
}

/**
 * Returns all objects in this object's inventory.
 *
 * @returns {STD_OBJECT*}
 */
object *query_contents () {
    return all_inventory();
}

/**
 * Returns all living objects in this object's inventory.
 *
 * @returns {STD_LIVING*}
 */
object *query_living_contents () {
    return filter(all_inventory(), (: livingp($1) :)); // TODO
}
/**
 * Returns all non-living item objects in this object's inventory.
 *
 * @returns {STD_ITEM*}
 */
object *query_item_contents () {
    return filter(all_inventory(), (: itemp($1) :));
}