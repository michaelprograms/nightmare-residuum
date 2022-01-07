void command (string input) {
    mixed *callouts;

    write(format_header_bar("CALLOUTS", mud_name()) + "\n\n");
    if (!sizeof(callouts = call_out_info())) {
        write("No pending callouts.\n");
    } else {
        write(sprintf("%:-40s %:-25s Delay", "Object", "Function") + "\n");
        foreach (mixed *callout in callouts) {
            callout[1] = replace_string(callout[1], "\n", "\\n");
            write(sprintf("%:-40O %:-25s %d\n", callout...) + "\n");
        }
    }
    write("\n" + format_footer_bar() + "\n");
}