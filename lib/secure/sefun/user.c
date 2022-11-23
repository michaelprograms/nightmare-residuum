string sanitize_name (string name);

object this_user () {
    object po = previous_object(-1)[<1];
    if (strsrch(D_TEST, base_name(po)) == 0) po = previous_object(-1)[<2];
    return po;
}

object this_account () {
    object a;

    if (this_user() && (a = this_user()->query_account())) return a;
    else return 0;
}
object this_character () {
    object c;
    foreach (object ob in ({ previous_object(), previous_object(1), efun::this_user() })) {
        if (ob && (c = ob->query_character())) {
            break;
        }
    }
    return c;
}
object find_character (string name) {
    object *results = filter(children(STD_CHARACTER), (: $1 && $1->query_key_name() == $2:), sanitize_name(name));
    return sizeof(results) ? results[0] : 0;
}

object *characters () {
    return map(filter(users() || ({}), (: $1 && interactive($1) && $1->query_character() :)) || ({}), (: $1->query_character() :)) || ({});
}

string query_account_setting (string setting) {
    object account;

    if (!(account = this_account())) return 0;

    return account->query_setting(setting);
}