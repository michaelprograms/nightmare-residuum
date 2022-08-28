inherit STD_WEAPON;

void create () {
    ::create();
    set_name("bronze dagger");
    set_id(({ "bronze dagger", "dagger" }));
    set_short("a bronze dagger");
    set_long("A flimsy bronze dagger.");
    set_type("knife");
    set_value(50);
    set_wc(2);
    set_autoload(2);
}