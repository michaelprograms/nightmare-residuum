#include "user.h"

inherit "/secure/user/account.c";
inherit "/secure/user/character.c";
inherit "/secure/user/input.c";
inherit "/secure/user/output.c";
inherit "/secure/user/shell.c";

#define CONNECT_TIMEOUT 60

nosave private int calloutBanner, calloutTimeout;

nosave private mapping __Terminal = ([
    "ip": 0,
    "width": 80,
    "height": 40,
    "type": 0,
    "encoding": "unknown",
    "color": "256",
    "client": "unknown",
]);

int is_user() { return 1; }

nomask void logon_banner () {
    if (!calloutBanner) {
        return;
    }
    calloutBanner = 0;

    receive_message("system", D_WELCOME->query_banner() + "\n");
    account_input();
    if (this_user() && interactive(this_user())) {
        telnet_ga(); // telnet go ahead for prompt to not line return
    }
}

/* --- interactive apply --- */

nomask void logon () {
    __Terminal["ip"] = query_ip_number();
    __Terminal["encoding"] = query_encoding();
    D_LOG->log("connect", ctime() + " " + __Terminal["ip"]);
    debug_message(ctime() + " connect from " + __Terminal["ip"]);

    receive_message("system", "Connecting...\n");
    calloutBanner = call_out_walltime((: logon_banner :), 0.5); // allow time for terminal_type apply to be called
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

void terminal_type (string term) {
    D_LOG->log_unique("term", term);

    term = lower_case(explode(term, " ")[0]);
    __Terminal["type"] = term;

    if (member_array(term, ({ "cmud", "zmud", })) > -1) {
        // set_encoding("US-ASCII"); // @TODO currently not working
        __Terminal["color"] = "16";
    } else if (member_array(term, ({ "mudslinger", })) > -1) {
        __Terminal["color"] = "16";
    }

    if (!undefinedp(calloutBanner)) { // force banner
        remove_call_out(calloutBanner);
        calloutBanner = -1;
        logon_banner();
    }
}

void receive_environ (string key, string value) {
    switch (key) {
        case "IPADDRESS":
            __Terminal["ip"] = value + " (" + __Terminal["ip"] + ")";
            break;
        case "CLIENT_NAME":
            __Terminal["client"] = value;
            break;
        // case "CLIENT_VERSION":
        //     break;
    }
}

void window_size (int width, int height) {
    __Terminal["width"] = width;
    __Terminal["height"] = height;
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
    } else if (type == "prompt") {
        receive(wrap(message));
    } else {
        message = format_message_color(type, message) + "\n";
        receive(wrap(message));
    }
}

/* --- interactive non-apply */

mixed query_terminal (string key) {
    return __Terminal[key];
}
void set_terminal (string key, mixed value) {
    __Terminal[key] = value;
}

string query_terminal_type () {
    return __Terminal["type"];
}
string query_terminal_color () {
    return __Terminal["color"];
}
void set_terminal_color (string color) {
    if (color != "256" && color != "16") {
        return;
    }
    __Terminal["color"] = color;
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