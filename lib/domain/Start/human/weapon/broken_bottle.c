inherit STD_WEAPON;

void create () {
    ::create();
    set_name("broken bottle");
    set_id(({ "broken bottle", "bottle" }));
    set_short("a broken bottle");
    set_long("A broken bottle with a sharp glass edge.");
    set_type("blade");
    set_value(2);
    set_wc(1);
}