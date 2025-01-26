
nosave private int __LevelReq = 0;

void set_autoload (int level) {
    __LevelReq = level;
}

mixed *query_autoload_data () {
    // override
    return ({ });
}

/**
 * Check the autoload requirements of this module against the character.
 *
 * @param {STD_CHARACTER} character the target of requirements
 */
mixed *query_autoload (object character) {
    if (character && character->query_level() >= __LevelReq && __LevelReq > 0) {
        return ({ base_name(), this_object()->query_autoload_data()... });
    } else {
        return 0;
    }
}

void restore_autoload (mixed *data) {
    // override
}