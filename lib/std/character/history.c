#define HISTORY_MAX 25

nosave string *__Tells = ({ });
nosave string __TellReply;
nosave string *__Says = ({ });

/* ----- tells  ----- */

private void initialize_history () {
    if (!arrayp(__Tells)) {
        __Tells = ({ });
    }
    if (!arrayp(__Says)) {
        __Says = ({ });
    }
}

string *query_tell_history () {
    initialize_history();
    return __Tells;
}
void add_tell_history (string msg) {
    initialize_history();

    if (!sizeof(msg)) {
        error("Bad argument 1 to history->add_tell_history");
    }

    __Tells += ({ msg });

    if (sizeof(__Tells) > HISTORY_MAX) {
        __Tells = __Tells[<HISTORY_MAX..];
    }
}

string query_tell_reply () {
    return __TellReply;
}
void set_tell_reply (string name) {
    __TellReply = name;
}

/* ----- says  ----- */

string *query_say_history () {
    return __Says;
}
void add_say_history (string msg) {
    initialize_history();

    if (!sizeof(msg)) {
        error("Bad argument 1 to history->add_say_history");
    }

    __Says += ({ msg });

    if (sizeof(__Says) > HISTORY_MAX) {
        __Says = __Says[<HISTORY_MAX..];
    }
}