inherit "/std/shadow.c";

int is_npc () {
    return objectp(query_shadow());
}
int is_living () {
    return objectp(query_shadow());
}

/* ---- messages ---- */

mixed *__ReceivedMessages = ({ });
mixed *query_received_messages () {
    return __ReceivedMessages;
}
// override from /std/npc.c
void receive_message (string type, string message) {
    __ReceivedMessages += ({ ({ type, message }) });
    if (query_shadow()) {
        query_shadow()->receive_message(type, message);
    }
}
void clear_received_messages () {
    __ReceivedMessages = ({ });
}

/* ---- commands ---- */

mixed *__ReceivedCommands = ({ });
mixed *query_received_commands () {
    return __ReceivedCommands;
}
// override from /std/living/command.c
varargs int handle_command (string command, int debug) {
    __ReceivedCommands += ({ command });
    if (query_shadow()) {
        query_shadow()->handle_command(command, debug);
    }
}