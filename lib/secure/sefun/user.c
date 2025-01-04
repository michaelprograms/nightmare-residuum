/**
 * Returns the current interactive user.
 *
 * @returns {STD_USER} the current user
 */
object this_user () {
    object po = previous_object(-1)[<1];
    if (regexp(base_name(po), D_TEST[0..<3])) {
        po = previous_object(-1)[<2];
    } else if (po->is_room()) {
        po = efun::this_user();
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
    object *results = filter(children(STD_USER), (: $1 && $1->query_key_name() == $(name) :));
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
    object *results = filter(children(STD_CHARACTER), (: $1 && $1->query_key_name() == SEFUN->sanitize_name($(name)) && $1->query_user() :));
    return sizeof(results) ? results[0] : 0;
}

/**
 * Checks if an object is a character.
 *
 * @param {STD_CHARACTER} ob the object to check
 */
int characterp (object ob) {
    return ob && /** @type {STD_CHARACTER} */(ob)->is_character();
}

/**
 * Returns the list of current interactive characters.
 *
 * @returns {STD_CHARACTER *} list of characters
 */
object *characters () {
    return map(filter(users() || ({ }), (: $1 && interactive($1) && $1->query_character() :)) || ({ }), (: $1->query_character() :)) || ({ });
}

/**
 * Query a user account setting.
 *
 * @param setting the setting's name
 * @returns the setting's value
 */
string query_account_setting (string name) {
    object user;
    if (!(user = this_user())) {
        return 0;
    }
    return user->query_setting(name);
}