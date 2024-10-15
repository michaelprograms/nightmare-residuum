inherit "/std/shadow.c";

mixed *__AutoloadData = ({ });

void set_autoload_data (mixed *data) {
    __AutoloadData = data;
}
mixed *query_autoload_data () {
    return __AutoloadData;
}