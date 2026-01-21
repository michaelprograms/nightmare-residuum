/**
 * Checks if an object is a living object.
 *
 * @param {STD_LIVING} ob the object to check
 * @returns 0 or 1
 */
int livingp (mixed ob) {
    if (!objectp(ob)) {
        return 0;
    }
    return !!(ob->is_living());
}

/**
 * Checks if an object is a NPC object.
 *
 * @param {STD_NPC} ob the object to check
 * @returns 0 or 1
 */
int npcp (mixed ob) {
    if (!objectp(ob)) {
        return 0;
    }
    return !!(ob->is_npc());
}

/**
 * Checks if an object is an NPC Vendor object.
 *
 * @param {STD_VENDOR} ob the object to check
 * @returns 0 or 1
 */
int vendorp (mixed ob) {
    if (!objectp(ob)) {
        return 0;
    }
    return !!(ob->is_vendor());
}

/**
 * Checks if an object is a character object.
 *
 * @param {STD_CHARACTER} ob the object to check
 * @returns 0 or 1
 */
int characterp (mixed ob) {
    if (!objectp(ob)) {
        return 0;
    }
    return !!(ob->is_character());
}

/**
 * Checks if an object is an immortal character object.
 *
 * @param {STD_CHARACTER} ob the object to check
 * @returns 0 or 1
 */
int immortalp (mixed ob) {
    if (!objectp(ob)) {
        return 0;
    }
    return !!(ob->query_immortal());
}

/**
 * Checks if an object is an item object.
 *
 * @param {STD_ITEM} ob the object to check
 * @returns 0 or 1
 */
int itemp (mixed ob) {
    if (!objectp(ob)) {
        return 0;
    }
    return !!(ob->is_item());
}

/**
 * Checks if an object is an armor object.
 *
 * @param {STD_ARMOR} ob the object to check
 * @returns 0 or 1
 */
int armorp (mixed ob) {
    if (!objectp(ob)) {
        return 0;
    }
    return !!(ob->is_armor());
}

/**
 * Checks if an object is a weapon object.
 *
 * @param {STD_WEAPON} ob the object to check
 * @returns 0 or 1
 */
int weaponp (mixed ob) {
    if (!objectp(ob)) {
        return 0;
    }
    return !!(ob->is_weapon());
}

/**
 * Checks if an object is a food object.
 *
 * @param {STD_FOOD} ob the object to check
 * @returns 0 or 1
 */
int foodp (mixed ob) {
    if (!objectp(ob)) {
        return 0;
    }
    return !!(ob->is_food());
}

/**
 * Checks if an object is a drink object.
 *
 * @param {STD_DRINK} ob the object to check
 * @returns 0 or 1
 */
int drinkp (mixed ob) {
    if (!objectp(ob)) {
        return 0;
    }
    return !!(ob->is_drink());
}

/**
 * Checks if an object is a room object.
 *
 * @param {STD_ROOM} ob the object to check
 * @returns 0 or 1
 */
int roomp (mixed ob) {
    if (!objectp(ob)) {
        return 0;
    }
    return !!(ob->is_room());
}