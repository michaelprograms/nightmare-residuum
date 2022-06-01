nosave private mixed __Long;
nosave private mixed __Short;

nomask void set_long (mixed m) {
    __Long = m;
}
string query_long () {
    return funcall(__Long);
}

nomask void set_short (mixed m) {
    __Short = m;
}
string query_short () {
    return funcall(__Short);
}