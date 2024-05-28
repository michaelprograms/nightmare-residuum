inherit "/std/shadow.c";

int is_living () {
    return objectp(query_shadow());
}

mixed *__ReceivedCommands = ({ });
mixed *query_received_commands () {
    return __ReceivedCommands;
}

varargs int handle_command (string command, int debug) {
    __ReceivedCommands += ({ command });
}