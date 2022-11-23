#include "user.h"

inherit "/secure/user/account.c";
inherit "/secure/user/character.c";
inherit "/secure/user/input.c";
inherit "/secure/user/output.c";
inherit "/secure/user/shell.c";

#define CONNECT_TIMEOUT 60

nosave private int calloutBanner, calloutTimeout;
nosave private string __TerminalType;
nosave private string __TerminalColor = "256";
nosave private string __IPAddr;

/* --- interactive apply --- */

nomask void logon () {
    debug_message(ctime() + " connect from " + query_ip_number()); // @TODO log_file
    D_LOG->log("connect", ctime() + " " + query_ip_number());

    receive_message("system", "Connecting...\n");
    calloutBanner = call_out_walltime(function () {
        if (!calloutBanner) {
            return;
        }
        receive_message("system", D_WELCOME->query_banner() + "\n");
        account_input();
        if (this_user() && interactive(this_user())) {
            telnet_ga(); // telnet go ahead for prompt to not line return
        }
    }, 0.5); // allow time for terminal_type apply to be called
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
    destruct();
}

void receive_message (string type, string message) {
    D_LOG->log_unique("message_types", type);
    type = lower_case(type);
    if (type == "raw ansi") {
        message = replace_string(message, "%^", "%%^^");
        receive(wrap(message, 0, 0, 1));
    } else if (type == "system") {
        if (strlen(message) > __LARGEST_PRINTABLE_STRING__) {
            message = message[0..__LARGEST_PRINTABLE_STRING__-1];
        }
        receive(wrap(message, 0, 0));
    } else {
        message = format_message_color(type, message);
        receive(wrap(message));
    }
}

void terminal_type (string term) {
    D_LOG->log_unique("term", term);

    __TerminalType = term;
    term = lower_case(explode(term, " ")[0]);

    if (member_array(term, ({ "cmud", })) > -1) {
        set_encoding("ascii");
    } else if (member_array(term, ({ "zmud", })) > -1) {
        set_encoding("ascii");
        __TerminalColor = "16";
    } else if (member_array(term, ({ "mudslinger", "xterm-256color", })) > -1) {
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

string query_terminal_type () {
    return __TerminalType;
}
string query_terminal_color () {
    return __TerminalColor;
}

nomask varargs void quit_character (int destructing) {
    message("system", "Reality "+(random(2)?"explodes into an im":"implodes into an ex")+"plosion of irreality.\n", this_object());
    character_exit();
    shell_stop();
    flush_messages();
    if (!destructing) {
        account_input(STATE_ACCOUNT_MENU);
    }
}

nomask void quit_account () {
    quit_character(1);
    handle_remove();
}

nomask void reset_connect_timeout () {
    if (!undefinedp(calloutTimeout)) remove_call_out(calloutTimeout);
    calloutTimeout = call_out((: handle_remove, "\nTime exceeded. Connection terminated.\n" :), CONNECT_TIMEOUT);
}

nomask varargs void handle_remove (string message) {
    if (!undefinedp(calloutTimeout)) remove_call_out(calloutTimeout);
    if (message) message("system", message, this_object());

    if (query_shell()) {
        shell_stop();
    }
    if (query_character()) {
        quit_character(1);
    }
    if (query_account()) {
        destruct(query_account());
    }

    flush_messages();
    destruct();
}