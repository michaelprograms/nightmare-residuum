nosave private function __EndFn;

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
    message("system", ed_cmd(s), this_user());
    if (query_ed_mode() == -1) {
        this_user()->input_pop();
        // this_user()->set_flag("editing", 0);
        if (__EndFn) {
            evaluate(__EndFn);
        }
        return destruct();
    }
}
void editor_start (string file, function endFn) {
    __EndFn = endFn;
    this_user()->input_push((: editor_input :), (: query_prompt :));
    ed_start(file, 0);
    // this_user()->set_flag("editing", 1); // @TODO
}

int clean_up (mixed *args...) {
    if (query_ed_mode() == -1) {
        destruct();
        return !this_object();
    } else {
        return 1;
    }
}
