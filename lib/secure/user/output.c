
void handle_pager (mixed arg) {
    string *lines;

    if (arrayp(arg)) lines = arg;
    else if (stringp(arg)) lines = explode(arg, "\n");

    new("/secure/std/pager.c")->start(lines, this_user());
}