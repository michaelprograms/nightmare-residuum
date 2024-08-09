inherit STD_ITEM;

void create () {
    ::create();
    set_name("fountain");
    set_id(({ "fountain", "basin", "fountain basin", }));
    set_adjective(({ "wide" }));
    set_short("a wide %^I_CYAN%^fountain basin%^DEFAULT%^");
    set_long("A fountain with a wide basin made of a hardened clay. A metallic winged figure stands upon a small pedestal in the center of the basin with water gently spraying out of its mouth.");
    set_no_get(1);
}