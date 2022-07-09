void command (string input, mapping flags) {
    message("system", "\n" + mud_name() + " is entering shutdown mode.\n\n", characters());
    shutdown();
}