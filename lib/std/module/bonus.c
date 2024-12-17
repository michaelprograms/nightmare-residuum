// Inherited by armor or weapons to keep track of bonuses

nosave mapping __Bonuses = ([ ]);

/**
 * Set the initial state of bonuses.
 */
private void initialize_bonuses () {
    if (!mapp(__Bonuses)) {
        __Bonuses = ([ ]);
    }
}

/* ----- querying ----- */

/**
 * Query a bonus name from the list of bonuses.
 *
 * @param key the key name to return
 * @returns the value stored to key
 */
mixed query_bonus (string key) {
    initialize_bonuses();
    return __Bonuses[key];
}
/**
 * Query the whole list of bonuses.
 * @returns all bonuses currently set
 */
mapping query_bonuses () {
    initialize_bonuses();
    return __Bonuses;
}

/* ----- setting ----- */

/**
 * Set a bonus.
 *
 * @param key the bonus' key name
 * @param value the bonus' value
 * @returns the value that was set
 */
mixed set_bonus (string key, mixed value) {
    initialize_bonuses();
    return __Bonuses[key] = value;
}
/**
 * Set several bonuses at once.
 *
 * @param bonuses
 * @returns all bonuses currently set
 */
mapping set_bonuses (mapping bonuses) {
    initialize_bonuses();
    foreach (string key, int value in bonuses) {
        set_bonus(key, value);
    }
    return __Bonuses;
}

/* ----- applying ----- */

/**
 * Apply a bonus to a target.
 *
 * @param {STD_LIVING} target to apply bonus to
 */
void apply_bonus (object target) {
    foreach (string key, int value in __Bonuses) {
        if (member_array(key, ({ "strength", "perception", "endurance", "charisma", "intelligence", "agility", "luck", })) > -1) {
            target->add_stat_bonus(key, value);
        }
    }
}

/* ----- removing ----- */

/**
 * Remove a bonus from a target.
 *
 * @param {STD_LIVING} target to remove bonus from
 */
void remove_bonus (object target) {
    foreach (string key, int value in __Bonuses) {
        if (member_array(key, ({ "strength", "perception", "endurance", "charisma", "intelligence", "agility", "luck", })) > -1) {
            target->add_stat_bonus(key, -value);
        }
    }
}