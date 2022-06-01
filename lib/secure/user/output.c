
void handle_pager (mixed arg) {
    string *lines;

    if (pointerp(arg)) lines = arg;
    else if (stringp(arg)) lines = explode(arg, "\n");

    clone_object("/secure/std/pager.c")->start(lines, this_user());
}