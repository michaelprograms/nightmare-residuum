inherit "/secure/user/account.c";
inherit "/secure/user/character.c";
inherit "/secure/user/input.c";
inherit "/secure/user/shell.c";

#include "user.h"

#define CONNECT_TIMEOUT 60

nosave private int calloutBanner, calloutTimeout;
nosave private string __TerminalType;
nosave private string __TerminalColor = "rgb";
nosave private string __IPAddr;

/* --- interactive apply --- */

nomask void logon () {
    debug_message(ctime() + " connect from " + query_ip_number()); // @TODO LOG_D

    calloutBanner = call_out_walltime(function () {
        if (!calloutBanner) {
            return;
        }
        receive_message("system", D_WELCOME->query_banner() + "\n");
        account_input();
        if (this_user() && interactive(this_user())) telnet_ga();
    }, 0.5); // allow time for terminal_type to be called
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

void terminal_type (string term) {
    __TerminalType = term;
    // @TODO LOG_D->log_unique(term);

    term = lower_case(explode(term, " ")[0]);
    if (term == "mudslinger") {
        __TerminalColor = "16";
    }

    if (!undefinedp(calloutBanner)) { // force prompt
        remove_call_out(calloutBanner);
        calloutBanner = 0;
        receive_message("system", D_WELCOME->query_banner() + "\n");
        account_input();
        if (this_user() && interactive(this_user())) telnet_ga();
    }
}

void receive_environ (string var, string value) {
    // Web clients send this
    // if (var == "IPADDRESS") {
    //     // @TODO use this instead of query_ip_address?
    // }
    // var can also be "CLIENT_NAME", "CLIENT_VERSION"

}

/* --- interactive non-apply */

string query_terminal_type() {
    return __TerminalType;
}
string query_terminal_color() {
    return __TerminalColor;
}

nomask void quit_character (int destructing) {
    message("system", "Reality "+(random(2)?"explodes into an im":"implodes into an ex")+"plosion of irreality.\n", this_object());
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
    if (!undefinedp(calloutTimeout)) remove_call_out(calloutTimeout);
    calloutTimeout = call_out((: handle_remove, "\nTime exceeded. Connection terminated.\n" :), CONNECT_TIMEOUT);
}

protected nomask int handle_login_commands (string input) {
    // Handle any commands from the login prompt here
    // if (grepp(input, "MSSP-REQUEST")) { /* from Dead Souls */ }
    // @TODO
    return 0;
}

nomask varargs void handle_remove (string message) {
    if (!undefinedp(calloutTimeout)) remove_call_out(calloutTimeout);
    if (message) message("system", message, this_object());
    if (query_shell()) shell_stop();
    if (query_character()) quit_character(1);
    flush_messages();
    destruct(this_object());
}