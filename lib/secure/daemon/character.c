#define DIR_SAVE_CHARACTER "/save/character"
#define VALID_TYPES ({ "character", "shell", })

int query_valid_name (string name) {
    int l;
    if (!name || (l = strlen(name)) < 4 || l > 18) return 0;
    if (regexp(name, "^[a-zA-Z]+[a-zA-Z\\ \\'\\-]+$")) return 1;
    return 0;
}

int query_valid_save_path (string name, string path) {
    string *results = pcre_extract(path, "^" + DIR_SAVE_CHARACTER + "/([a-z])/([a-z]+)/([a-z]+)");
    return results[0] == name[0..0] && results[1] == name && member_array(results[2], VALID_TYPES) > -1;
}
varargs string query_save_path (string name, string type) {
    if (strlen(name) < 4) error("Bad argument 1 to character->query_save_path");
    if (undefinedp(type)) error("Bad argument 2 to character->query_save_path");
    return DIR_SAVE_CHARACTER + "/" + name[0..0] + "/" + name + "/" + type + ".o";
}

int query_exists (string name) {
    return query_valid_name(name) && unguarded((: file_size, query_save_path(name, "character") :)) > -1;
}

object query_whois_character (string name) {
    object char;

    if (!query_exists(name)) return 0;

    char = clone_object(STD_CHARACTER);
    char->set_name(capitalize(name));
    char->restore_data();
    char->set_save_path(0);
    return char;
}