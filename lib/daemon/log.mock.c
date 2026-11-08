inherit "/std/shadow.c";

nosave private mixed *__Logs = ({});
mixed *query_logs() {
    return __Logs;
}

// override from /daemon/log.c, records instead of writing to /log
int log(string filename, string entry) {
    __Logs += ({ ({ filename, entry }) });
    return 1;
}
