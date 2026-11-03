// inherits the instrumented copy so hits count toward command.c coverage
// @lpc-ignore
inherit "/std/living/command.coverage.c";

nosave private int __Immobile;
nosave private string __Posture = "standing";
nosave private mixed *__Messages = ({});

void set_immobile(int n) {
    __Immobile = n;
}
int query_immobile() {
    return __Immobile;
}

void set_posture(string posture) {
    __Posture = posture;
}
string query_posture() {
    return __Posture;
}

mixed *query_received_messages() {
    return __Messages;
}
void receive_message(string type, string message) {
    __Messages += ({ ({ type, message }) });
}

// the bare module has no way to move itself
void mock_move(object dest) {
    move_object(dest);
}

void create() {
    parse_init();
}
