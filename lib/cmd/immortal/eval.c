#define ED_BASIC_COMMANDS "\"%^CYAN%^BOLD%^i%^RESET%^\"nsert code, \"%^CYAN%^BOLD%^.%^RESET%^\" to save, e\"%^CYAN%^BOLD%^x%^RESET%^\"ecute, \"%^CYAN%^BOLD%^q%^RESET%^\"uit to abort"

inherit STD_COMMAND;

void create () {
    set_syntax("eval ([LPC commands])");
}

void clear_file (string file);
void execute_file (string file, string input);
void create_tmp_file (string file, string input);

void end_edit () {
    string tmpFile = user_path(this_character()->query_key_name());
    string input;

    if (file_size(tmpFile) != -2) {
        write("You must have a valid home directory.\n");
        return;
    }
    tmpFile += "/CMD_EVAL_FILE.c";
    if (!write_file(tmpFile, "")) {
        write("You must have write access.\n");
        return;
    }
    execute_file(tmpFile, input);
}

void command (string input, mapping flags) {
    string tmpFile = user_path(this_character()->query_key_name());

    if (file_size(tmpFile) != -2) {
        message("system", "You must have a valid home directory.\n", this_user());
        return;
    }
    tmpFile += "/CMD_EVAL_FILE.c";
    if (!write_file(tmpFile, "")) {
        message("system", "You must have write access.\n", this_user());
        return;
    }

    if (input) {
        if (!regexp(input, ";$")) {
            input = input + ";";
        }
        if (regexp(input, ";")) {
            input = replace_string(input, "; ", ";\n");
        }
        clear_file(tmpFile);
        execute_file(tmpFile, input);
    } else {
        string tmp;
        message("system", "Entering eval ed mode, standard ed commands apply:\n", this_user());
        message("system", ED_BASIC_COMMANDS + "\n", this_user());
        message("system", "__________________________________________________________________________\n", this_user());
        if (tmp = read_file(tmpFile)) {
            message("system", tmp + "\n", this_user());
        }
        clear_file(tmpFile);
        new("/secure/std/editor.c")->editor_start(tmpFile, (: end_edit :));
    }
    return;
}

void execute_file (string file, string input) {
    mixed ret;
    int timeBefore, timeAfter;

    create_tmp_file(file, input);

    timeBefore = perf_counter_ns();
    ret = call_other(file, "eval");
    timeAfter = perf_counter_ns();

    if (regexp(input, "return")) {
        write("Result (%^ORANGE%^" + sprintf("%.2f", (timeAfter - timeBefore)/1000000.0) + " ms%^RESET%^) = " + identify(ret)+"\n");
    } else {
        write("Complete (%^ORANGE%^" + sprintf("%.2f", (timeAfter - timeBefore)/1000000.0) + " ms%^RESET%^)\n");
    }
}

void clear_file (string file) {
    mixed ret;
    rm(file);
    if (ret = find_object(file)) destruct(ret);
}

void create_tmp_file (string file, string input) {
    string lines = @EndCode
#define TU this_user()
#define TC this_character()
#define TO this_object()
#define ENVTC environment(this_character())

inherit M_CLEAN;

EndCode;
    lines += "mixed eval() {\n    " + input + "\n}";

    write_file(file, lines);
}