inherit STD_ITEM;

void create () {
    ::create();
    set_name("fountain");
    set_id(({ "fountain", "basin", "fountain basin", }));
    set_adjective(({ "grand" }));
    set_short("a grand %^CYAN%^fountain basin%^DEFAULT%^");
    set_long("A fountain with a wide basin made of a hardened clay. Numerous metallic winged figures stands in a circle upon a pedestal in the center of the basin with water gently spraying out of their mouths.");
    set_no_get(1);
}