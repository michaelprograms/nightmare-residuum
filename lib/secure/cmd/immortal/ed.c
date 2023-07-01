inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("ed [file]");
    set_help_text("The ed command is used to enter edit mode on a file.");
}

nosave private mapping __Locks = ([ ]);
nomask private void unlock (string file) {
    map_delete(__Locks, file);
}

void command (string input, mapping flags) {
    string cwd, file;
    int size;

    if (!input) {
        write("Syntax: ed [file]\n");
        return;
    }
    cwd = this_user()->query_shell()->query_variable("cwd");
    file = absolute_path(input, cwd);

    if (objectp(__Locks[file])) {
        write("ed: " + file + " locked by " + __Locks[file]->query_name() + ".\n");
        return;
    }

    switch (size = file_size(file)) {
        case -2:
            write("ed: " + file + ": not a file.\n");
            return;
        case -1:
            write("ed: " + file + ": new file.\n");
            break;
        default:
            write("ed: " + file + ": " + sizeof(explode(read_file(file), "\n")) + " lines, " + size + " bytes.\n");
            break;
    }

    __Locks[file] = this_character();

    new("/secure/std/editor.c")->editor_start(file, (: unlock($(file)) :));
}