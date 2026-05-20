#define DIR_SAVE_ACCOUNT "/save/account"

inherit M_CLEAN;

int query_valid_name (string name) {
    int l;
    if (!name || (l = sizeof(name)) < 4 || l > 64) {
        return 0;
    }
    if (regexp(name, "^[a-zA-Z0-9]+[a-zA-Z0-9_@\\.\\-]+$")) {
        return 1;
    }
    return 0;
}

string query_save_path (string name) {
    if (!query_valid_name(name)) {
        return 0;
    }
    return sprintf(DIR_SAVE_ACCOUNT+"/%c/%s.o", name[0], name);
}

int query_exists (string name) {
    string path = query_save_path(name);
    return path && file_size(path) > -1;
}