#include <driver/runtime_config.h>

mapping fds;

void read_call_back (int fd, mixed mess) { }
void write_call_back (int fd) { }
void close_call_back (int fd) {
    string file, *lines;

    if (undefinedp(fds[fd]) || !objectp(fds[fd])) return;

    file = "/tmp/" + fds[fd]->query_name();
    lines = explode(read_file(file), "\n");
    foreach (string line in lines) {
        message("action", line + "\n", fds[fd]);
    }

    rm(file);
    map_delete(fds, fd);
}

void command (string input, mapping flags) {
    string cwd, file;
    string *params;
    int fd;

    if (!input) {
        write("Syntax: cat [file]\n");
        return;
    }
    cwd = this_user()->query_shell()->query_variable("cwd");
    file = absolute_path(input, cwd);
    switch (file_size(file)) {
        case -2:
            write("cat: " + file + ": not a file.\n");
            return;
        case -1:
            write("cat: " + file + ": no such file.\n");
            return;
    }

    params = ({ "-l","pike","-O","style=native,linenos=1","-f","terminal256","-o",get_config(__MUD_LIB_DIR__)+"/tmp/"+this_character()->query_name(),get_config(__MUD_LIB_DIR__)+file });
    write("params: "+implode(params, " ")+"\n");
    fd = external_start(1, params, (:read_call_back:), (:write_call_back:), (:close_call_back:));
    if (fd < 0) {
        write("cat: external call failed with error: "+fd+".\n");
        return;
    }
    if (!mapp(fds)) fds = ([]);
    fds[fd] = this_character();
}