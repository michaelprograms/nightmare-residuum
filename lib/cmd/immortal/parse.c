void command (string input) {
    write("Sending --"+input+"-- to parser...\n");
    this_character()->do_command(input, 2);
}