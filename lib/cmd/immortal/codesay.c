void command (string input) {
    if (!input || input == "") {
        write("Codesay what?\n");
        return;
    }

    input = replace_string(input, ";", ";\n");
    input = replace_string(input, "{", "{\n");
    input = replace_string(input, "}", "}\n");
    if (input[<2..<1] == "\n") {
        input = input[0..<2];
    }

    write_file("/tmp/codesay", input, 1);
    ed_start("/tmp/codesay");
    ed_cmd("I");
    ed_cmd("x");
    input = read_file("/tmp/codesay")[0..<2];
    input = "\n// " + replace_string(input, "\n", "\n// ") + "\n";

    message("no_ansi", "You codesay:" + input, this_character());
    message("no_ansi", this_character()->query_name() + " codesays:" + input, environment(this_character()), this_character());
}