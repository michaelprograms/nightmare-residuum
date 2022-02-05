nosave private mixed __Long;
nosave private mixed __Short;

nomask void set_long (mixed m) {
    __Long = m;
}
nomask string query_long () {
    return evaluate(__Long);
}

nomask void set_short (mixed m) {
    __Short = m;
}
nomask string query_short () {
    return evaluate(__Short);
}