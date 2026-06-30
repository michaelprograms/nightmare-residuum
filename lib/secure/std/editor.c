nosave private function __EndFn;
/** @type {STD_USER} __User */
nosave private object __User;

nomask private string query_prompt () {
    switch (query_ed_mode()) {
        case 0:
            return ":";
        // mode 1 = line input, -2 = executing macro, others = unknown
        case -2: default:
            return "*";
    }
}

private nomask void editor_input (mixed s) {
    // s == -1 means the connection dropped
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

/**
 * Start the line editor on a file, calling endFn when the session closes.
 *
 * @param {string} file - path to the file to edit
 * @param {function} endFn - optional callback invoked after a clean exit
 */
void editor_start (string file, function endFn) {
    __EndFn = endFn;
    __User = this_user();
    __User->input_push((: editor_input :), (: query_prompt :));
    ed_start(file, 0);
}

/* ----- ed setup ----- */

/**
 * Query the user's ed setup flags.
 *
 * @returns {int} the setup bitmask stored on the user object
 */
int query_ed_setup () {
    return __User->query_ed_setup();
}

/**
 * Set the user's ed setup flags.
 *
 * @param {int} code - setup bitmask to store on the user object
 */
void set_ed_setup (int code) {
    __User->set_ed_setup(code);
}

/* ----- applies ----- */

/**
 * Driver clean_up apply. Destructs the editor if ed has exited; returns 1 to
 * keep the object alive while an edit session is still active.
 *
 * @returns {int} 0 when destructed, 1 when kept alive
 */
int clean_up (mixed args...) {
    if (query_ed_mode() == -1) {
        destruct();
        return !this_object();
    } else {
        return 1;
    }
}
