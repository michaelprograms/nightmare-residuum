inherit STD_ITEM;

mixed direct_wield_obj (object ob, string str) {
    return (environment() == previous_object());
}