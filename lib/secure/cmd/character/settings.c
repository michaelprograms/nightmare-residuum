inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("settings ([setting] [value])|([player])");
    set_help_text("The settings command is used to view or change your account's settings. Immortals may view another player's settings by providing a player name.");
}

/**
 * Display the settings table for a target.
 *
 * @param {STD_USER} user the user whose settings to display
 * @param {STD_CHARACTER} target the character for the subtitle
 */
private void display_settings (object user, object target) {
    mixed *settings = ({ });
    string *list = ({ });

    foreach (string key, mixed value in user->query_settings()) {
        settings += ({ ({ key, value }) });
    }
    settings = sort_array(settings, (: strcmp($1[0], $2[0]) :));
    foreach (string *setting in settings) {
        list += ({ sprintf("%16s : %s", setting[0], "" + setting[1]) });
    }

    border(([
        "title": "SETTINGS",
        "subtitle": target->query_cap_name(),
        "body": ([
            "items": list,
            "columns": 1,
        ]),
    ]));
}

/**
 * Validate and apply a setting change.
 *
 * @param {STD_USER} user the user to modify
 * @param key the setting key
 * @param value the setting value
 */
private void apply_setting (object user, string key, string value) {
    mixed converted = value;

    if (member_array(key, keys(user->query_settings())) == -1) {
        message("action", "Invalid setting: " + key, user);
        return;
    }

    switch (key) {
        case "width": case "lines":
            converted = to_int(value);
            if (converted < 1) {
                message("action", "Setting '" + key + "' must be a positive number.", user);
                return;
            }
            break;
        case "autojoin":
            if (member_array(value, user->query_character_names()) == -1) {
                message("action", "You must provide a valid character name for autojoin.", user);
                return;
            }
            break;
        case "ansi": case "gmcp": case "screenreader":
            if (member_array(value, ({ "on", "off" })) == -1) {
                message("action", "Setting '" + key + "' must be 'on' or 'off'.", user);
                return;
            }
            break;
    }

    user->set_setting(key, converted);
    message("action", "Setting %^BOLD%^" + key + "%^BOLD_OFF%^ to %^BOLD%^" + converted + "%^BOLD_OFF%^.", user);
}

void command (string input, mapping flags) {
    object tc = this_character(), user;
    string *split;

    user = tc->query_user();

    if (!input) {
        display_settings(user, tc);
        return;
    }

    split = explode(input, " ");

    if (sizeof(split) == 1 && tc->query_immortal()) {
        object target;
        target = determine_immortal_target(tc, split[0]);
        if (target != tc) {
            display_settings(target->query_user(), target);
            return;
        }
        // Fall through: not a valid target, treat as missing value
    }

    if (sizeof(split) < 2) {
        message("action", "Usage: settings [setting] [value]", user);
        return;
    }

    apply_setting(user, split[0], implode(split[1..], " "));
}
