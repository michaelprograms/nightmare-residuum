#define DIR_SAVE_ACCOUNT "/save/account"

int query_valid_name (string name) {
    int l;
    if (!name || (l = strlen(name)) < 4 || l > 64) return 0;
    if (regexp(name, "^[a-zA-Z0-9]+[a-zA-Z0-9_@\\.\\-]+$")) return 1;
}

string query_save_path (string name) {
    if (strlen(name) < 4) error("Bad argument 1 to account->query_save_path");
    return sprintf(DIR_SAVE_ACCOUNT+"/%c/%s.o", name[0], name);
}

int query_exists (string name) {
    return query_valid_name(name) && file_size(query_save_path(name)) > -1;
}