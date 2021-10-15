nosave private string __SavePath;

string query_save_path () {
    return __SavePath;
}
void set_save_path (string path) {
    if (undefinedp(path) || (!stringp(path) && path != 0)) {
        error("Bad argument 1 to save->set_save_path");
    }
    __SavePath = path;
}

void restore_data () {
    if (stringp(__SavePath)) {
        unguarded((: restore_object, __SavePath :));
    }
}
void save_data () {
    if (stringp(__SavePath)) {
        unguarded((: save_object, __SavePath :));
    }
}