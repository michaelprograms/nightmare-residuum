inherit "/std/shadow.c";

/* ---- settings ---- */

nosave private mapping __Settings = ([]);
void set_setting(string key, mixed value) {
    __Settings[key] = value;
}
// override from /std/user/account.c
mixed query_setting(string key) {
    return __Settings[key];
}

/* ---- character ---- */

nosave private object __Character;
void set_character(object ob) {
    __Character = ob;
}
// override from /std/user/character.c
object query_character() {
    return __Character;
}
