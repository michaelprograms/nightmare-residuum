inherit "/secure/module/http.c";

void create () {
    ::create();
    set_port(6669);
    add_url_pattern("^/app/who", "app.who.data");
    start();
}