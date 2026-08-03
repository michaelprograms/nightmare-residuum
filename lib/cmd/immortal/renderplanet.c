inherit STD_COMMAND;

void create() {
    ::create();
    set_syntax("renderplanet <name>");
    set_help_text(
        "Render a planet's map straight to lib/tmp/<name>-<size>.png using the "
        "native renderer (no JSON, no external tooling). Requires the FFI "
        "noise library to be loaded."
    );
}

void command(string input, mapping flags) {
    if (!stringp(input) || !sizeof(input)) {
        write("Usage: renderplanet <name>\n");
        return;
    }
    D_PLANET->generate_png(input);
}
