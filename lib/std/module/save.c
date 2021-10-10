nosave private string __SavePath;

protected string query_save_path () {
    return __SavePath;
}
protected void set_save_path (string path) { // @TODO private for security?
    if (undefinedp(path) || !stringp(path)) {
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