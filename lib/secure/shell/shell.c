#include <driver/origin.h>
#include "shell.h"

inherit M_SAVE;

inherit "/secure/shell/alias.c";

nosave private object __User;
nosave private mapping __ShellCommands = ([]);

void create () {
    if (!clonep()) {
        return;
    }

    __User = previous_object();
    if (__User != this_user()) {
        destruct();
        error("illegal shell object created?");
    }

    if (__User) {
        string keyName = __User->query_character()->query_key_name();
        set_save_path(sprintf("/save/shell"+"/%c/%s.o", keyName[0], keyName));
        restore_data(); // @TODO check if exists first?
    }

    alias::create();
}

void handle_remove () {
    if (origin() != ORIGIN_LOCAL && __User && __User != previous_object()) {
        error("illegal attempt to remove shell object?");
    }

    save_data();
    destruct();
}

void execute_command (string input) {
    string *split = explode(input, " ");
    string action = split[0], path;
    string args = sizeof(split) > 1 ? input[(strlen(action)+1)..] : 0;

    split -= ({ "" });


    // Channel
    // @TODO D_CHANNEL

    if (path = D_COMMAND->query_command(action)) {
        call_other(path+"/"+action, "command", args); // @TODO is this right?
    } else if (!__User->query_character()->do_command(input)) {
        write("$ What?\n");
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
    // @TODO
    // @TODO why disabled?
}

protected mixed query_prompt () {
    return "> ";
}

void shell_start () {
    if (__User != this_user() || __User != previous_object()) {
        error("illegal attempt to take over shell?");
    }

    this_user()->input_push((: shell_input :), query_prompt());
    shell_init();
}