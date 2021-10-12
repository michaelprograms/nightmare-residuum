inherit "/secure/user/account.c";
inherit "/secure/user/character.c";
inherit "/secure/user/input.c";
inherit "/secure/user/shell.c";

#include "user.h"

#define CONNECT_TIMEOUT 60

nosave private int calloutHandle;

/* --- interactive apply --- */

nomask void logon () {
    debug_message(ctime()+" connect() from "+query_ip_number()); // @TODO LOG_D

    call_out(function() {
        receive_message("system", D_WELCOME->query_banner() + "\n");
        account_input();
        if (this_user() && interactive(this_user())) telnet_ga();
    }, 0);
}

nomask void net_dead () {
    if (query_account() && query_character()) {
        query_account()->update_character_data(query_character());
        character_linkdead();
    }
    if (query_account()) {
        destruct(query_account());
    }
    if (query_shell()) {
        destruct(query_shell());
    }
}

void receive_message (string type, string message) {
    if (type == "system") {
        receive(wrap(message, 0, 0));
    } else if (type == "say") {
        message = "%^CYAN%^" + replace_string(message, ":", ":%^RESET%^");
        receive(wrap(message));
    } else if (type == "go") {
        message = "%^GREEN%^BOLD%^" + message + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "room_living_contents") {
        message = "%^RED%^BOLD%^" + message + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "room_nonliving_contents") {
        message = "%^MAGENTA%^" + message + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "room_exits") {
        message = "%^GREEN%^" + replace_string(message, ":", ":%^RESET%^") + "%^RESET%^";
        receive(wrap(message));
    } else {
        receive(wrap(message, 0, 0));
    }
}

/* --- interactive non-apply */

nomask void reconnect () {
    // @TODO - what calls this?
}

nomask void quit_character (int destructing) {
    write("Reality "+(random(2)?"explodes into an im":"implodes into an ex")+"plosion of irreality.\n");
    // @TODO query_character()->handle_connection_status("quit");
    character_exit();
    shell_stop();
    flush_messages();
    if (!destructing) {
        account_input(STATE_ACCOUNT_MENU);
    }
}

nomask void quit_account () {
    quit_character(1);
    destruct(this_object());
}

nomask void reset_connect_timeout () {
    if (!undefinedp(calloutHandle)) remove_call_out(calloutHandle);
    calloutHandle = call_out((: handle_remove, "\nTime exceeded. Connection terminated.\n" :), CONNECT_TIMEOUT);
}

protected nomask int handle_login_commands (string input) {
    // Handle any commands from the login prompt here
    // if (grepp(input, "MSSP-REQUEST")) { /* from Dead Souls */ }
    // @TODO
    return 0;
}

nomask varargs void handle_remove (string message) {
    if (!undefinedp(calloutHandle)) remove_call_out(calloutHandle);
    if (message) message("system", message, this_object());
    // @TODO destruct account/character/shell
    flush_messages();
    destruct(this_object());
}