nosave private function __EndFn;
nosave private object __User;

nomask private string query_prompt () {
    switch (query_ed_mode()) {
        case 0:
            return ":";
        case -2: default:
            return "*";
    }
}

private nomask void editor_input (mixed s) {
    if (s == -1) {
        return destruct();
    }
    message("raw ansi", ed_cmd(s), __User);
    if (query_ed_mode() == -1) {
        __User->input_pop();
        if (__EndFn) {
            evaluate(__EndFn);
        }
        return destruct();
    }
}
void editor_start (string file, function endFn) {
    __EndFn = endFn;
    __User = this_user();
    __User->input_push((: editor_input :), (: query_prompt :));
    ed_start(file, 0);
}

/* ----- ed setup ----- */

void query_ed_setup () {
    return __User->query_ed_setup();
}
int set_ed_setup (int code) {
    __User->set_ed_setup(code);
}

/* ----- applies ----- */

int clean_up (mixed *args...) {
    if (query_ed_mode() == -1) {
        destruct();
        return !this_object();
    } else {
        return 1;
    }
}
