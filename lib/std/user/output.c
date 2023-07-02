void handle_pager (mixed arg) {
    string *lines;

    if (stringp(arg)) {
        lines = explode(arg, "\n");
    } else if (arrayp(arg)) {
        lines = arg;
    }

    new("/secure/std/pager.c")->start(lines, this_object());
}