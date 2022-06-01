
inherit "/secure/user/account.c";
inherit "/secure/user/character.c";
inherit "/secure/user/input.c";
inherit "/secure/user/output.c";
inherit "/secure/user/shell.c";

#include "user.h"

#define CONNECT_TIMEOUT 60

nosave private int calloutBanner, calloutTimeout;
nosave private string __TerminalType;
nosave private string __TerminalColor = "256";
nosave private string __IPAddr;

/* --- interactive apply --- */

private void calloutbannerwelcome () {
    // if (!calloutBanner) {
    //     return;
    // }
    receive_message("system", D_WELCOME->query_banner() + "\n");
    account_input();
    // if (this_user() && interactive(this_user())) telnet_ga(); // @LDMUD disabled
}

void logon () {
    // @LDMUD disabled
    // debug_message(ctime() + " connect from " + query_ip_number()); // @TODO log_file
    // D_LOG->log("connect", ctime() + " " + query_ip_number());

    receive_message("system", "Connecting...\n\n");
    receive_message("system", D_WELCOME->query_banner() + "\n");
    account_input();

    // calloutbannerwelcome();
    // calloutBanner = call_out((: calloutbannerwelcome() :), 1); // _walltime
    // allow time for terminal_type to be called
}

void net_dead () {
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

// @LDMUD shim
private void receive (string msg) {
    tell_object(this_object(), msg);
}

void receive_message (string type, string message) {
    if (type == "system") {
        receive(wrap(message, 0, 0));
    } else if (type == "wrap" || type == "verb" || type == "action") {
        receive(wrap(message));
    } else if (type == "say") {
        message = "%^CYAN%^" + regreplace(message, ":", ":%^RESET%^", 1);
        receive(wrap(message));
    } else if (type == "go") {
        message = regreplace(message, "%^DIR%^", "%^CYAN%^", 1);
        message = regreplace(message, "%^DEFAULT%^", "%^GREEN%^", 1);
        message = "%^GREEN%^BOLD%^" + message + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "room_living_contents") {
        message = "%^RED%^" + regreplace(message, "%^DEFAULT%^", "%^RED%^", 1) + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "room_item_contents") {
        message = "%^MAGENTA%^" + regreplace(message, "%^DEFAULT%^", "%^MAGENTA%^", 1) + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "room_long" || type == "room_look") {
        receive(wrap(message));
    } else if (type == "room_exits") {
        message = "%^GREEN%^" + regreplace(regreplace(message, "%^DEFAULT%^", "%^GREEN%^", 1), ":", ":%^RESET%^", 1) + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "channel") {
        message = regreplace(message, "[[", "%^GREEN%^[%^BOLD%^", 1);
        message = regreplace(message, "]]", "%^BOLD_OFF%^GREEN%^]%^RESET%^", 1);
        message = regreplace(message, "((", "%^CYAN%^(%^BOLD%^", 1);
        message = regreplace(message, "))", "%^BOLD_OFF%^CYAN%^)%^RESET%^", 1);
        receive(wrap(message));
    } else if (type == "combat hit") {
        message = "%^RED%^" + message + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "combat miss") {
        message = "%^GREEN%^" + message + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "no_ansi") {
        if (sizeof(message) > __LARGEST_PRINTABLE_STRING__) {
            message = message[0..__LARGEST_PRINTABLE_STRING__ - 1];
        }
        receive(message);
    } else {
        receive(wrap(message, 0, 0));
    }
}

// void terminal_type (string term) {
//     D_LOG->log_unique("term", term);

//     __TerminalType = term;
//     term = lower_case(explode(term, " ")[0]);

//     if (member(term, ({ "cmud", })) > -1) {
//         set_encoding("ascii");
//     } else if (member(term, ({ "zmud", })) > -1) {
//         set_encoding("ascii");
//         __TerminalColor = "16";
//     } else if (member(term, ({ "mudslinger", "xterm-256color", })) > -1) {
//         __TerminalColor = "16";
//     }

//     if (calloutBanner) { // force prompt
//         remove_call_out(calloutBanner);
//         calloutBanner = 0;
//         receive_message("system", D_WELCOME->query_banner() + "\n");
//         account_input();
//         if (this_user() && interactive(this_user())) telnet_ga();
//     }
// }

// void receive_environ (string var, string value) {
//     // Web clients send this
//     // if (var == "IPADDRESS") {
//     //     // @TODO use this instead of query_ip_address?
//     // }
//     // var can also be "CLIENT_NAME", "CLIENT_VERSION"

// }

/* --- interactive non-apply */

string query_terminal_type () {
    return __TerminalType;
}
string query_terminal_color () {
    return __TerminalColor;
}

varargs void quit_character (int destructing) {
    message("system", "Reality "+(random(2)?"explodes into an im":"implodes into an ex")+"plosion of irreality.\n", this_object());
    character_exit();
    shell_stop();
    // flush_messages();
    if (!destructing) {
        account_input(STATE_ACCOUNT_MENU);
    }
}

void quit_account () {
    quit_character(1);
    handle_remove();
}

varargs void reset_connect_timeout (int stop) {
    if (calloutTimeout) {
        calloutTimeout = 0;
        remove_call_out("handle_remove");
    }
    if (!stop) {
        call_out((: handle_remove("\nTime exceeded. Connection terminated.\n") :), CONNECT_TIMEOUT);
        calloutTimeout = 1;
    }
}

varargs void handle_remove (string message) {
    if (calloutTimeout) remove_call_out("handle_remove");
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

    // flush_messages();
    destruct(this_object());
}