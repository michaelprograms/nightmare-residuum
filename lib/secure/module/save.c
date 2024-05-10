nosave private string __SavePath = 0;

private int __Created = time();

/* -----  ----- */

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

/* -----  ----- */

int query_created () {
    return __Created;
}

/* -----  ----- */

void restore_data () {
    if (stringp(__SavePath) && file_size(__SavePath) > 0) {
        restore_object(__SavePath);
    }
}
void save_data () {
    if (stringp(__SavePath)) {
        mkdirs(__SavePath);
        save_object(__SavePath);
    }
}