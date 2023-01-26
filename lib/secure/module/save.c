nosave private string __SavePath = 0;

string query_save_path () {
    return __SavePath;
}
void set_save_path (string path) {
    if (stringp(path) && regexp(path, "^\\/save\\/")) {
        __SavePath = path;
    } else {
        __SavePath = 0;
    }
}

void restore_data () {
    if (stringp(__SavePath) && unguarded((: file_size, __SavePath :)) > 0) {
        unguarded((: restore_object, __SavePath :));
    }
}
void save_data () {
    if (stringp(__SavePath)) {
        mkdirs(__SavePath);
        unguarded((: save_object, __SavePath :));
    }
}