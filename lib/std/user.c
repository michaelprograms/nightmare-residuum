#include "/std/user/user.h"

inherit "/std/user/account.c";
inherit "/std/user/character.c";
inherit "/std/user/gmcp.c";
inherit "/std/user/input.c";
inherit "/std/user/output.c";
inherit "/std/user/shell.c";

#define CONNECT_TIMEOUT 60

nosave private int calloutConnect;

nosave private mapping __Terminal;

void receive_message (string type, string message);
nomask varargs void handle_remove (string message);

int is_user () {
    return 1;
}

private void initialize_terminal () {
    if (!mapp(__Terminal)) {
        __Terminal = ([
            "ip": 0,
            "width": 80,
            "height": 40,
            "type": 0,
            "encoding": "unknown",
            "color": "256",
            "client": "unknown",
            "version": "unknown",
        ]);
    }
}

nomask void logon_banner () {
    receive_message("system", query_banner() + "\n");
    account_input(STATE_ACCOUNT_ENTER);
    if (this_user() && interactive(this_user())) {
        telnet_ga(); // telnet go ahead for prompt to not line return
    }
}

/* ----- interactive apply ----- */

nomask void logon () {
    initialize_terminal();
    __Terminal["ip"] = query_ip_number();
    __Terminal["encoding"] = query_encoding();
    D_LOG->log("connect", ctime() + " " + __Terminal["ip"]);
    D_CHARACTER->increment_connected_total();

    receive_message("system", "%^RESET%^Connected...\n");
    call_out_walltime((: logon_banner :), 0.5); // allow time for terminal_type apply to be called
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
}

void receive_environ (string key, string value) {
    switch (key) {
    case "IPADDRESS":
        __Terminal["ip"] = value + " (" + __Terminal["ip"] + ")";
        break;
    case "CLIENT_NAME":
        __Terminal["client"] = value;
        break;
    case "CLIENT_VERSION":
        __Terminal["version"] = replace_string(value, "/", ".");
        break;
    }
}

void window_size (int width, int height) {
    __Terminal["width"] = width;
    __Terminal["height"] = height;
}

nomask void net_dead () {
    if (query_name() && query_character()) {
        character_linkdead();
    }
    destruct();
}

void receive_message (string type, string message) {
    type = lower_case(type);
    if (type == "raw ansi") {
        message = replace_string(message, "%^", "%%^^");
        receive(wrap(message, 0, 0, 1));
    } else if (type == "system") {
        if (sizeof(message) > __LARGEST_PRINTABLE_STRING__) {
            message = message[0..__LARGEST_PRINTABLE_STRING__-1];
        }
        receive(wrap(message, 0, 0));
    } else if (type == "prompt") {
        receive(wrap(message));
    } else if (type == "pager" || sizeof(message) > __LARGEST_PRINTABLE_STRING__ / 2) {
        handle_pager(message);
    } else {
        message = format_message_color(type, message) + "\n";
        receive(wrap(message));
    }
}

/* ----- interactive non-apply ----- */

mixed query_terminal (string key) {
    initialize_terminal();
    return __Terminal[key];
}
void set_terminal (string key, mixed value) {
    initialize_terminal();
    __Terminal[key] = value;
}

string query_terminal_type () {
    initialize_terminal();
    return __Terminal["type"];
}
string query_terminal_color () {
    initialize_terminal();
    return __Terminal["color"];
}
void set_terminal_color (string color) {
    initialize_terminal();
    if (color != "256" && color != "16") {
        return;
    }
    __Terminal["color"] = color;
}

nomask varargs void quit_character (int destructing) {
    message("system", "Reality "+(random(2)?"explodes into an im":"implodes into an ex")+"plosion of irreality.\n", this_object());
    character_exit();
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
    if (!undefinedp(calloutConnect)) {
        remove_call_out(calloutConnect);
    }
    calloutConnect = call_out((: handle_remove, "\nTime exceeded. Connection terminated.\n" :), CONNECT_TIMEOUT);
}

nomask varargs void handle_remove (string message) {
    if (!undefinedp(calloutConnect)) {
        remove_call_out(calloutConnect);
    }
    if (message) {
        message("system", message, this_object());
    }

    if (query_character()) {
        quit_character(1);
    }

    flush_messages();
    destruct();
}

void create () {
    account::create();
    shell::create();
}