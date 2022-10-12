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
    if (type == "system") {
        receive(wrap(message, 0, 0));
    } else if (member_array(type, ({ "action", "story", "verb", "wrap", })) > -1) {
        receive(wrap(message));
    } else if (type == "say") {
        message = "%^CYAN%^" + replace_string(message, ":", ":%^RESET%^");
        receive(wrap(message));
    } else if (type == "tell") {
        message = "%^RED%^BOLD%^" + replace_string(message, ":", ":%^RESET%^");
        receive(wrap(message));
    } else if (type == "go") {
        message = replace_string(message, "%^DIR%^", "%^CYAN%^");
        message = replace_string(message, "%^DEFAULT%^", "%^GREEN%^");
        message = "%^GREEN%^BOLD%^" + message + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "room_listen") {
        message = "%^CYAN%^" + message + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "room_smell") {
        message = "%^ORANGE%^" + message + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "room_living_contents") {
        message = "%^RED%^" + replace_string(message, "%^DEFAULT%^", "%^RED%^") + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "room_item_contents") {
        message = "%^MAGENTA%^" + replace_string(message, "%^DEFAULT%^", "%^MAGENTA%^") + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "room_long" || type == "room_look") {
        receive(wrap(message));
    } else if (type == "room_exits") {
        message = "%^GREEN%^" + replace_string(replace_string(message, "%^DEFAULT%^", "%^GREEN%^"), ":", ":%^RESET%^") + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "channel") {
        message = replace_string(message, "[[", "%^GREEN%^[%^BOLD%^");
        message = replace_string(message, "]]", "%^BOLD_OFF%^GREEN%^]%^RESET%^");
        message = replace_string(message, "((", "%^CYAN%^(%^BOLD%^");
        message = replace_string(message, "))", "%^BOLD_OFF%^CYAN%^)%^RESET%^");
        receive(wrap(message));
    } else if (type == "combat hit") {
        message = "%^RED%^" + message + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "combat miss") {
        message = "%^GREEN%^" + message + "%^RESET%^";
        receive(wrap(message));
    } else if (type == "combat alert") {
        message = "%^YELLOW%^" + message + "%^RESET%^";
        receive(wrap(message));
    // } else if (type == "combat action") {
    } else if (type == "no_ansi") {
        if (strlen(message) > __LARGEST_PRINTABLE_STRING__) {
            message = message[0..__LARGEST_PRINTABLE_STRING__ - 1];
        }
        receive(message);
    } else {
        receive(wrap(message, 0, 0));
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