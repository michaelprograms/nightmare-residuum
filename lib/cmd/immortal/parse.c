void command (string input) {
    if (!input) {
        write("parse: input required.\n");
        return;
    }
    write("Sending '"+input+"' to parser...\n");
    this_character()->do_command(input, 2);
}