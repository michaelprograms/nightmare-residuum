// #define TMP_FILE "/tmp/CMD_EVAL_EDIT."+this_character()->query_key_name()+".eval"
#define ED_BASIC_COMMANDS "\"%^CYAN%^BOLD%^i%^RESET%^\"nsert code, \"%^CYAN%^BOLD%^.%^RESET%^\" to save, e\"%^CYAN%^BOLD%^x%^RESET%^\"ecute, \"%^CYAN%^BOLD%^q%^RESET%^\"uit to abort"

void clear_file (string file);
void execute_file (string file, string input);
void create_tmp_file (string file, string input);

// void end_edit (mixed *args) {
//     string file = args[0];
//     string tmpFile = TMP_FILE;
//     string input = read_file(tmpFile);
//     clear_file(file);
//     execute_file(file, input);
// }
// void abort () { }

void command (string input) {
    string file;

    file = "/tmp/CMD_EVAL_FILE." + this_character()->query_key_name() + ".c";
    if (!write_file(file, "")) {
        write("You must have write access.\n");
        return;
    }

    if (input) {
        if (!regexp(input, ";$")) input = input + ";";
        if (regexp(input, ";")) input = replace_string(input, "; ", ";\n");
        clear_file(file);
        execute_file(file, input);
    // } else {
    //     string tmp;
    //     write("Entering eval ed mode, standard ed commands apply:\n");
    //     write(ED_BASIC_COMMANDS+"\n");
    //     write("__________________________________________________________________________\n");
    //     if (tmp = read_file(TMP_FILE)) write(tmp+"\n"); // this_player()->catch_tell(tmp);
    //     // this_player()->edit(TMP_FILE, (:end_edit:), (:abort:), ({file}));
    }
    return;
}

void execute_file (string file, string input) {
    mixed ret;
    int t = 0;
    create_tmp_file(file, input);
    t = rusage()["utime"] + rusage()["stime"];
    ret = (mixed)call_other(file, "eval");
    t = rusage()["utime"] + rusage()["stime"] - t;
    if (regexp(input, "return")) write("Result (%^ORANGE%^"+t+" ms%^RESET%^) = " + identify(ret)+"\n");
    else write ("Complete (%^ORANGE%^"+t+" ms%^RESET%^)\n");
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
EndCode;
    lines += "mixed eval() {\n    " + input + "\n}";

    write_file(file, lines);
}

// void help() {
//     write( //ABILITY->format_utility(
//         "eval ([LPC commands])",
//         "Creates a temporary file containing [LPC commands]%^RESET%^ which is executed with call_other and the results are returned. If [LPC commands]%^RESET%^ are not input, places the user in edit mode.\n\n"
//         "Examples:\n"
//         "> eval return 1 + cos( 0.0 )\n"
//         "Result = 2.000000\n"
//         "> eval return explode(\"banana\", \"a\")\n"
//         "Result = ({ \"b\", \"n\", \"n\" })\n\n"
//         "> eval\n"
//         "Entering eval ed mode, standard ed commands apply:\n"
//         ED_BASIC_COMMANDS+"\n"
//         "__________________________________________________________________________\n"
//         "> i"
//         "> object ob = new(FOOD);\n"
//         "> ob->set_strength(100);\n"
//         "> return ob->query_strength();\n"
//         "> .\n"
//         "> x\n"
//         "\"tmp/CREATOR_NAME.eval\" 3 lines 75 bytes"
//         "Result = 100"
//         "Exit from ed."
//     );
//     //     , 1
//     // ));
// }
