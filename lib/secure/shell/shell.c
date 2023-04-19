#include <driver/origin.h>
#include "shell.h"

inherit M_CLEAN;
inherit M_PARENT;
inherit M_SAVE;

inherit "/secure/shell/alias.c";
inherit "/secure/shell/variable.c";

nosave private object __Owner;
nosave private mapping __ShellCommands = ([]);

object query_character () {
    return __Owner && __Owner->query_character();
}

void create () {
    if (!clonep()) {
        return;
    }

    __Owner = previous_object() || previous_object(1);
    if (__Owner != this_user() && !regexp(file_name(__Owner), "\\.test")) {
        error("illegal shell object created: " + file_name(__Owner));
        destruct();
    }

    if (__Owner && __Owner->query_character()) {
        set_save_path(D_CHARACTER->query_save_path(__Owner->query_character()->query_key_name(), "shell"));
        restore_data();
    }

    parent::create();
    alias::create();
    variable::create();
}

void handle_remove () {
    if (origin() != ORIGIN_LOCAL && __Owner && __Owner != previous_object()) {
        error("Invalid origin in call to shell->handle_remove");
    }

    save_data();
    destruct();
}

// Parse the raw input for any command line flags provided
// Flags must be at the beginning of raw input and in format:
// -f           or      -flag
// -f=text      or      -flag=text
// -f=123.0     or      -flag=123.0
mixed *parse_command_flags (string rawInput) {
    string *args, input = "";
    mapping flags = ([ ]);
    int x, y, l;

    if (!stringp(rawInput)) {
        input = rawInput;
    } else {
        args = explode(rawInput, " ");
        l = sizeof(args);

        for (int i = 0; i < l; i ++) {
            if (regexp(args[i], "^-")) {
                if ((x = strlen(args[i])) > 1) {
                    if ((y = strsrch(args[i], "=")) > -1) {
                        flags[args[i][1..y-1]] = args[i][y+1..];
                    } else {
                        flags[args[i][1..]] = 1;
                    }
                }
            } else {
                input = implode(args[i..], " ");
                break;
            }
        }
    }

    return ({ input, flags });
}

void execute_command (string command) {
    string *split, action, input;
    string cmdPath;

    if (!command) return;

    split = explode(command, " ") - ({ "" });
    action = split[0];
    input = sizeof(split) > 1 ? command[(strlen(action)+1)..] : 0;

    if (D_CHANNEL->query_valid_channel(action)) {
        return D_CHANNEL->send(action, query_character(), input);
    }

    // @TODO move to STD_LIVING->do_command
    if (cmdPath = D_COMMAND->query_command(action)) {
        mixed *parse = parse_command_flags(input);
        catch(call_other(cmdPath + "/" + action, "command", parse[0], parse[1]));
    } else if (__Owner->query_character() && !__Owner->query_character()->do_command(command)) {
        write("Do what?\n");
    }
}

protected void shell_input (mixed input) {
    if (input == -1) return handle_remove();

    input = trim(input);
    if (input == "") return;

    // @TODO history

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

protected void shell_init () {
    set_variable("cwd", "/", 1);
    set_variable("prompt", "$hp/$HP hp $sp/$SP sp $mp/$MP mp > ", 1);
}

protected mixed query_prompt () {
    object tc = query_parent()->query_character();
    string prompt = query_variable("prompt");
    int i;

    while ((i = strsrch(prompt, "$")) != -1) {
        if (i + 2 > strlen(prompt) - 1) break;
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

void shell_start () {
    if ((__Owner != this_user() || __Owner != previous_object()) && !regexp(file_name(__Owner), "\\.test$")) {
        error("illegal attempt to take over shell?");
    }

    this_user()->input_push((: shell_input :), (: query_prompt :));
    shell_init();
}

int clean_up (mixed *args...) {
    if (__Owner) {
        return clean::clean_later();
    } else {
        return clean::clean_up();
    }
}