
nosave private int __LevelReq = 0;

void set_autoload (int level) {
    __LevelReq = level;
}


mixed *query_autoload_data () {
    return ({ });
}

mixed *query_autoload (object character) {
    if (character && character->query_level() >= __LevelReq) {
        return ({ base_name(this_object()), query_autoload_data()... });
    } else {
        return 0;
    }
}

void restore_autoload (mixed *data) {
    // override
}