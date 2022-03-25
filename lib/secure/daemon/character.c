#define DIR_SAVE_CHARACTER "/save/character"

int query_valid_name (string name) {
    int l;
    if (!name || (l = strlen(name)) < 4 || l > 18) return 0;
    if (regexp(name, "^[a-zA-Z]+[a-zA-Z\\ \\'\\-]+$")) return 1;
    return 0;
}

string query_save_path (string name) {
    if (strlen(name) < 4) error("Bad argument 1 to character->query_save_path");
    return sprintf(DIR_SAVE_CHARACTER+"/%c/%s.o", name[0], name);
}

int query_exists (string name) {
    return query_valid_name(name) && unguarded((: file_size, query_save_path(name) :)) > -1;
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