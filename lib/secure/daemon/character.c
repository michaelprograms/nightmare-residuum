#define DIR_SAVE_CHARACTER "/save/character"
#define VALID_TYPES ({ "character", "pet", })

inherit M_CLEAN;

int query_valid_name (string name) {
    int l;
    if (!name || (l = strlen(name)) < 4 || l > 18) {
        return 0;
    }
    if (regexp(name, "^[a-zA-Z]+[a-zA-Z\\ \\'\\-]+$")) {
        return 1;
    }
    return 0;
}

varargs string query_save_path (string name, string type) {
    int l;
    if (!name || (l = sizeof(name)) < 4 || l > 64) {
        return 0;
    }
    if (!stringp(type) || !sizeof(type)) {
        type = "character";
    }
    return DIR_SAVE_CHARACTER + "/" + name[0..0] + "/" + name + "/" + type + ".o";
}

varargs int query_exists (string name, string type) {
    if (!stringp(type) || !sizeof(type)) {
        type = "character";
    }
    return query_valid_name(name) && file_size(query_save_path(name, type)) > -1;
}

/**
 * Loads a character's data.
 *
 * @param name which caracter to load
 * @returns {STD_CHARACTER}
 */
private object load_character (string name) {
    object char;

    if (!query_exists(name)) {
        return 0;
    }
    char = new(STD_CHARACTER);
    char->set_name(capitalize(name));
    char->restore_data();
    char->set_save_path(0);
    return char;
}

/**
 * Queries an off-line character's data.
 *
 * @param name which character to query
 * @returns {STD_CHARACTER}
 */
object query_character (string name) {
    return load_character(name);
}

int query_immortal (string name) {
    object char;
    int immortal = 0;

    char = load_character(name);
    if (char) {
        immortal = char->query_immortal();
        destruct(char);
    }
    return immortal;
}