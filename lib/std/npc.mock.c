inherit "/std/shadow.c";

int is_npc() {
    return objectp(query_shadow());
}
int is_living() {
    return objectp(query_shadow());
}

/* ---- messages ---- */

mixed *__ReceivedMessages = ({});
mixed *query_received_messages() {
    return __ReceivedMessages;
}
// override from /std/npc.c
void receive_message(string type, string message) {
    __ReceivedMessages += ({ ({ type, message }) });
    if (query_shadow()) {
        /** @type {STD_NPC} */ (query_shadow())->receive_message(type, message);
    }
}
void clear_received_messages() {
    __ReceivedMessages = ({});
}

/* ---- receive ---- */

// when set, the shadowed object refuses to receive any object, forcing the
// handle_move failure branch in code under test
int __RefuseReceive = 0;
void set_refuse_receive(int i) {
    __RefuseReceive = i;
}
// override from /std/module/container.c
int can_receive(object ob) {
    if (__RefuseReceive) {
        return 0;
    }
    if (query_shadow()) {
        return /** @type {STD_NPC} */ (query_shadow())->can_receive(ob);
    }
    return 1;
}

/* ---- commands ---- */

mixed *__ReceivedCommands = ({});
mixed *query_received_commands() {
    return __ReceivedCommands;
}
// override from /std/living/command.c
varargs int handle_command(string command, int debug) {
    __ReceivedCommands += ({ command });
    if (query_shadow()) {
        /** @type {STD_NPC} */ (query_shadow())->handle_command(command, debug);
    }
    return 1;
}
