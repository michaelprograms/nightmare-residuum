#define ED_BASIC_COMMANDS "\"%^CYAN%^BOLD%^i%^RESET%^\"nsert code, \"%^CYAN%^BOLD%^.%^RESET%^\" to save, e\"%^CYAN%^BOLD%^x%^RESET%^\"ecute, \"%^CYAN%^BOLD%^q%^RESET%^\"uit to abort"

inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("eval ([LPC commands])");
    set_help_text(
        "Creates a temporary file containing [LPC commands]%^RESET%^ which is executed with call_other and the results are returned. If [LPC commands]%^RESET%^ are not input, places the user in edit mode.\n\n"
        "Examples:\n"
        "> eval return 1 + cos(0.0)\n"
        "Result = 2.000000\n"
        "> eval return explode(\"banana\", \"a\")\n"
        "Result = ({ \"b\", \"n\", \"n\" })\n\n"
        "> eval\n"
        "Entering eval ed mode, standard ed commands apply:\n"
        ED_BASIC_COMMANDS+"\n"
        "________________________________________________________________________________\n"
        ":i\n"
        "float f = 1 + sin(0.0);\n"
        "return f;\n"
        ".\n"
        ":x\n"
        "\"/realm/name/.eval.tmp\" 2 lines 34 bytes\n"
        "Exit from ed.\n"
        "Result = 1.000000"
    );
}

void clear_file (string file);
void execute_file (string file, string input);
void create_tmp_file (string file, string input);

void end_edit (string evalfile, string tmpfile) {
    string input;
    object ob;

    input = read_file(tmpfile);
    if (ob = find_object(evalfile)) {
        ob->handle_remove();
    }
    execute_file(evalfile, input);
}

void command (string input, mapping flags) {
    string userpath = user_path(this_character()->query_key_name());
    string evalfile, tmpfile;
    string tmp;

    if (file_size(userpath) != -2) {
        message("system", "You must have a valid home directory.\n", this_user());
        return;
    }

    evalfile = userpath + "/CMD_EVAL_FILE.c";
    if (!write_file(evalfile, "")) {
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
        execute_file(evalfile, input);
    } else {
        tmpfile = userpath + "/.eval.tmp";
        message("system", "Entering eval ed mode, standard ed commands apply:\n", this_user());
        message("system", ED_BASIC_COMMANDS + "\n", this_user());
        message("system", "________________________________________________________________________________\n", this_user());
        if (tmp = read_file(tmpfile)) {
            message("system", tmp + "\n", this_user());
        }
        new("/secure/std/editor.c")->editor_start(tmpfile, (: end_edit($(evalfile), $(tmpfile)) :));
    }
    return;
}

void execute_file (string file, string input) {
    mixed ret;
    int timeBefore, timeAfter;

    clear_file(file);
    create_tmp_file(file, input);

    timeBefore = perf_counter_ns();
    ret = call_other(file, "eval");
    timeAfter = perf_counter_ns();

    if (regexp(input, "return")) {
        write("Result (%^ORANGE%^" + sprintf("%.2f", (timeAfter - timeBefore)/1000000.0) + " ms%^RESET%^) = " + identify(ret)+"\n");
    } else {
        write("Complete (%^ORANGE%^" + sprintf("%.2f", (timeAfter - timeBefore)/1000000.0) + " ms%^RESET%^)\n");
    }
    if (ret = find_object(file)) destruct(ret);
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