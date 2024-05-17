inherit "/std/shadow.c";

int is_npc () {
    return objectp(query_shadow());
}
int is_living () {
    return objectp(query_shadow());
}

mixed *__ReceivedMessages = ({ });
mixed *query_received_messages () {
    return __ReceivedMessages;
}

// override from /std/npc.c
void receive_message (string type, string message) {
    __ReceivedMessages += ({ ({ type, message }) });
}