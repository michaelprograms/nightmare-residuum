/**
 * Checks if an object is a character object.
 *
 * @param {STD_CHARACTER} ob the object to check
 * @returns 0 or 1
 */
int characterp (mixed ob) {
    return objectp(ob) && /** @type {STD_CHARACTER} */(ob)->is_character();
}

/**
 * Checks if an object is a living object.
 *
 * @param {STD_LIVING} ob the object to check
 * @returns 0 or 1
 */
int livingp (mixed ob) {
    return objectp(ob) && /** @type {STD_LIVING} */(ob)->is_living();
}