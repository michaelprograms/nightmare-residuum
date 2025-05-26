inherit STD_ITEM;

void create () {
    ::create();
    set_name("vat");
    set_id(({"vat", "brewery vat"}));
    set_short("a brewery vat");
    set_long("A large vat used for brewing beer. The metallic cylinder is elevated on several legs, with a funnel shape at the bottom.");
    set_no_get(1);
}
