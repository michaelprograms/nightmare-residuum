inherit "/secure/module/http.c";

void create() {
    ::create();
    set_port(16669);
    add_url_pattern("^/app/who", "app.who.data");
    add_url_pattern("^/app/planet/(\\w+)", "app.planet.png");
    start();
}
