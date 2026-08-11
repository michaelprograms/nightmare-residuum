inherit STD_COMMAND;

void create() {
    ::create();
    set_syntax(
        "createplanet -f(orce) <name> size=<n> [humidityFactor=<f>] "
        "[heatFactor=<f>] [heightFactor=<f>]"
    );
    set_help_text(
        "Create a new planet and persist it, then render it with 'renderplanet "
        "<name>'. size is required and must be a positive integer. The factors "
        "are multipliers on the humidity/heat/height noise (default 1.00; 0.0 "
        "flattens that channel). Existing planets are not overwritten unless you "
        "pass -force, which retunes the planet in place with the given values "
        "(flags must come before the name, ex: 'createplanet -force Luna "
        "humidityFactor=0.5')."
    );
}

void command(string input, mapping flags) {
    string *parts, name;
    mapping config = ([]);

    if (!stringp(input) || !sizeof(input)) {
        write(
            "Usage: createplanet [-force] <name> size=<n> "
            "[humidityFactor=<f>] ...\n"
        );
        return;
    }
    parts = explode(input, " ") - ({ "" });
    if (sizeof(parts) < 2) {
        write(
            "Usage: createplanet [-force] <name> size=<n> "
            "[humidityFactor=<f>] ...\n"
        );
        return;
    }
    name = parts[0];
    foreach (string kv in parts[1..]) {
        string *pair = explode(kv, "=");
        if (sizeof(pair) != 2) {
            write("Bad argument '" + kv + "' (expected key=value).\n");
            return;
        }
        if (pair[0] == "size") {
            config["size"] = to_int(pair[1]);
        } else {
            config[pair[0]] = to_float(pair[1]);
        }
    }
    if (D_PLANET->create_planet(name, config)) {
        write(
            "Created planet '" + name + "'. Render it with: renderplanet " +
            name + "\n"
        );
        return;
    }
    // create won't overwrite an existing planet; -force retunes it in place.
    if (flags["force"] && D_PLANET->adjust_planet(name, config)) {
        write(
            "Adjusted planet '" + name + "'. Render it with: renderplanet " +
            name + "\n"
        );
        return;
    }
    write(
        "Failed to create '" + name + "' (size missing/invalid" + (flags["force"]
            ? "" : ", or already exists - use -force to retune") + ").\n"
    );
}
