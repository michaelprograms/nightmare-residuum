#include <driver/origin.h>
#include "shell.h"

inherit M_SAVE;

inherit "/secure/shell/alias.c";
inherit "/secure/shell/variable.c";

nosave private object __Owner;
nosave private mapping __ShellCommands = ([]);

void create () {
    if (!clonep()) {
        return;
    }

    __Owner = previous_object() || previous_object(1);
    if (__Owner != this_user() && !regexp(file_name(__Owner), "\\.test$")) {
        error("illegal shell object created?");
        destruct();
    }

    if (__Owner) {
        string keyName = __Owner->query_character()->query_key_name();
        set_save_path(sprintf("/save/shell"+"/%c/%s.o", keyName[0], keyName));
        restore_data();
    }

    alias::create();
    variable::create();
}

void handle_remove () {
    if (origin() != ORIGIN_LOCAL && __Owner && __Owner != previous_object()) {
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
    } else if (!__Owner->query_character()->do_command(input)) {
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
    set_variable("cwd", "/", 1);
}

protected mixed query_prompt () {
    return "> ";
}

void shell_start () {
    if ((__Owner != this_user() || __Owner != previous_object()) && !regexp(file_name(__Owner), "\\.test$")) {
        error("illegal attempt to take over shell?");
    }

    this_user()->input_push((: shell_input :), query_prompt());
    shell_init();
}

void set_cwd (string path) {
    set_variable("cwd", path);
}