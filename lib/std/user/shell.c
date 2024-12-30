// @this_object /std/user.c
private mapping __Aliases = ([
    /* Data format:
        "t": string     // template
        "d": string *   // defaults
        "n": int
    */
]);
private string *__XAliases = ({ });
private mapping __Variables = ([ ]);
nosave private mapping __VariableHooks = ([ ]);
nosave private string *__History = ({ });

void initialize_shell ();

/* ----- aliases ----- */

nomask string *query_alias_names () {
    return keys(__Aliases);
}
nomask mapping query_alias (string alias) {
    return __Aliases[alias];
}

varargs void set_alias (string name, string template, string *defaults, int xverb) {
    int i;
    string *tmp;
    mapping new_alias;

    if (sizeof(__Aliases) >= 100) {
        write("You have reached the maximum amount of aliases.\n");
        return;
    }

    if (strsrch(template, "$*") == -1) {
        template += " $*";
    }

    tmp = explode(template[strsrch(template, "$")..], "$");

    new_alias = ([
        "t": template,
        "d": defaults,
        "n": max(map(tmp, function (string s) {
            int d;
            sscanf(s, "%d%s", d, s);
            return d;
        })),
    ]);

    if (!arrayp(new_alias["d"])) {
        new_alias["d"] = ({ });
    }
    i = new_alias["n"] - (sizeof(defaults) - 1);
    while (i--) {
        new_alias["d"] += ({ "" });
    }

    if (xverb) {
        __XAliases += ({ name });
    }
    __Aliases[name] = new_alias;
    this_object()->save_data();
}

nomask void unset_alias (string name) {
    map_delete(__Aliases, name);
    __XAliases -= ({ name });
}

mixed expand_alias (string input) {
    string *argv = explode(input, " ");
    mapping currentAlias;
    string *xverbMatches, expandedInput;
    int numArgs, i, j;

    initialize_shell();
    numArgs = sizeof(argv) - 1;
    xverbMatches = filter(__XAliases || ({ }), (: strsrch($2, $1) == 0 :), argv[0]);
    if (sizeof(xverbMatches) > 1) {
        error("Alias conflict: can't distinguish between " + implode(xverbMatches, ", "));
    } else if (sizeof(xverbMatches) == 1) {
        sscanf(argv[0], xverbMatches[0] + "%s", argv[0]);
        if (argv[0] == "") {
            argv[0] = xverbMatches[0];
        } else {
            numArgs ++;
            argv = xverbMatches + argv;
        }
    }

    if (!(currentAlias = __Aliases[argv[0]])) {
        return trim(implode(argv, " "));
    }

    expandedInput = replace_string(currentAlias["t"], "\\\\$", sprintf("%c", 255));

    for (i = 1, j = numArgs; i <= currentAlias["n"]; i ++, j --) {
        if (j < 1) {
            expandedInput = replace_string(expandedInput, sprintf("$%d", i), currentAlias["d"][i]);
        } else {
            expandedInput = replace_string(expandedInput, sprintf("$%d", i), argv[i]);
        }
    }
    if (j > 0) {
        expandedInput = replace_string(expandedInput, "$*", implode(argv[i..], " "));
    } else {
        expandedInput = replace_string(expandedInput, "$*", currentAlias["d"][0]);
    }

    return trim(replace_string(expandedInput, sprintf("%c", 255), "$"));
}

/* ----- variables ----- */

mixed query_variable (string v) {
    return __Variables && __Variables[v];
}
varargs mixed set_variable (string v, mixed value, int set_if_undefined) {
    int changed = 0;
    if (undefinedp(set_if_undefined) || (set_if_undefined && !__Variables[v])) {
        changed = (__Variables[v] != value);
        __Variables[v] = value;
        if (changed) {
            this_object()->save_data();
            if (__VariableHooks[v]) {
                evaluate(__VariableHooks[v], value);
            }
        }
    }
    return __Variables[v];
}
int unset_variable (string v) {
    map_delete(__Variables, v);
    this_object()->save_data();
    return !__Variables[v];
}

void set_variable_hook (string v, function fn) {
    __VariableHooks[v] = fn;
}

/* ----- shell ----- */

void execute_command (string command) {
    string *split, action, input;
    object character;

    if (!command) {
        return;
    }

    split = explode(command, " ") - ({ "" });
    action = split[0];
    input = sizeof(split) > 1 ? command[(sizeof(action)+1)..] : 0;

    if (D_CHANNEL->query_valid_channel(action)) {
        return D_CHANNEL->send(action, this_object()->query_character(), input);
    }

    if (character = this_object()->query_character()) {
        character->set_last_action();
        if (!character->handle_command(command)) {
            write("Do what?\n");
        }
    }
}

protected void shell_input (mixed input) {
    if (input == -1) {
        return;
    }

    input = trim(input);
    if (input == "") {
        return;
    }

    __History += ({ input });
    if (sizeof(__History) > 40) {
        __History = __History[<40..];
    }

    // bypass alias expand
    if (input[0] == '\\') {
        input = input[1..];
    } else {
        input = expand_alias(input);
    }
    if (input != "") {
        execute_command(input);
    }
}

protected mixed query_prompt () {
    object tc = this_object()->query_character();
    string prompt = query_variable("prompt") || "";
    int i;

    while ((i = strsrch(prompt, "$")) != -1) {
        if (i + 2 > sizeof(prompt) - 1) {
            break;
        }
        switch (prompt[i+1..i+2]) {
            case "hp":
                prompt = replace_string(prompt, "$hp", ""+tc->query_hp());
                break;
            case "sp":
                prompt = replace_string(prompt, "$sp", ""+tc->query_sp());
                break;
            case "mp":
                prompt = replace_string(prompt, "$mp", ""+tc->query_mp());
                break;
            case "HP":
                prompt = replace_string(prompt, "$HP", ""+tc->query_max_hp());
                break;
            case "SP":
                prompt = replace_string(prompt, "$SP", ""+tc->query_max_sp());
                break;
            case "MP":
                prompt = replace_string(prompt, "$MP", ""+tc->query_max_mp());
                break;
            case "cd":
                prompt = replace_string(prompt, "$cd", query_variable("cwd"));
                break;
            default:
                prompt = replace_string(prompt, "$", "");
        }
    }
    return prompt + " ";
}

string *query_history () {
    return __History;
}

/* ----- applies ----- */

void initialize_shell () {
    if (!mapp(__Variables)) {
        __Variables = ([ ]);
        set_variable("cwd", "/", 1);
        set_variable("prompt", "$hp/$HP hp $sp/$SP sp $mp/$MP mp > ", 1);
    }
    if (!mapp(__VariableHooks)) {
        __VariableHooks = ([ ]);
    }
    if (!sizeof(__XAliases)) {
        __XAliases = ({ });
    }
    if (!sizeof(__Aliases)) {
        __Aliases = ([ ]);
        set_alias("l", "look");
        set_alias("n", "go north");
        set_alias("ne", "go northeast");
        set_alias("e", "go east");
        set_alias("se", "go southeast");
        set_alias("s", "go south");
        set_alias("sw", "go southwest");
        set_alias("w", "go west");
        set_alias("nw", "go northwest");
        set_alias("u", "go up");
        set_alias("d", "go down");
        set_alias("ent", "go enter");
        set_alias("cl", "climb");
        set_alias("'", "say", 0, 1);
    }
}

void create () {
    initialize_shell();
}

protected nomask void shell_start () {
    this_object()->input_push((: shell_input :), (: query_prompt :));
}