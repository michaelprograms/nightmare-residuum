/**
 * Returns the current interactive user.
 *
 * @returns {STD_USER} the current user
 */
object this_user () {
    object po = previous_object(-1)[<1];
    if (base_name(po) == D_TEST[0..<3]) {
        po = previous_object(-1)[<2];
    }
    return po;
}

/**
 * Finds an interactive user account by name.
 *
 * @param name the name of the account
 * @returns {STD_USER} a matching user
 */
object find_user (string name) {
    if (!stringp(name)) {
        error("Bad argument 1 to user->find_user");
    }
    object *results = children(STD_USER);
    results = filter(results, (: $1 && $1->query_key_name() == $(name) :));
    return sizeof(results) ? results[0] : 0;
}

/**
 * Returns the current interactive character.
 *
 * @returns {STD_CHARACTER} the current character
 */
object this_character () {
    object c;
    foreach (object ob in ({ previous_object(), previous_object(1), efun::this_user() })) {
        if (ob && (c = ob->query_character())) {
            break;
        }
    }
    return c;
}

/**
 * Finds an interactive character by name.
 *
 * @param name the name of the character
 * @returns {STD_CHARACTER} a matching character
 */
object find_character (string name) {
    if (!stringp(name)) {
        error("Bad argument 1 to user->find_character");
    }
    object *results = children(STD_CHARACTER);
    results = filter(results, (: $1 && $1->query_key_name() == SEFUN->sanitize_name($(name)) && $1->query_user() :));
    return sizeof(results) ? results[0] : 0;
}

/**
 * Returns the list of current interactive characters.
 *
 * @returns {STD_CHARACTER *} list of characters
 */
object *characters () {
    object *users = users() || ({ });
    users = filter(users, (: $1 && interactive($1) && $1->query_character() :));
    return map(users, (: $1->query_character() :)) || ({ });
}

/**
 * Query a user account setting.
 *
 * @param setting the setting's name
 * @returns the setting's value, or 0 if no current user
 */
string query_account_setting (string setting) {
    object user = this_user();
    return user ? user->query_setting(setting) : 0;
}