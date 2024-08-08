string sanitize_name (string name);

object this_user () {
    object po = previous_object(-1)[<1];
    if (regexp(base_name(po), D_TEST[0..<3])) {
        po = previous_object(-1)[<2];
    } else if (po->is_room()) {
        po = efun::this_user();
    }
    return po;
}
object find_user (string name) {
    object *results = filter(children(STD_USER), (: $1 && $1->query_key_name() == $(name) :));
    return sizeof(results) ? results[0] : 0;
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
    object *results = filter(children(STD_CHARACTER), (: $1 && $1->query_key_name() == sanitize_name($(name)) && $1->query_user() :));
    return sizeof(results) ? results[0] : 0;
}

object *characters () {
    return map(filter(users() || ({ }), (: $1 && interactive($1) && $1->query_character() :)) || ({ }), (: $1->query_character() :)) || ({ });
}

string query_account_setting (string setting) {
    object user;

    if (!(user = this_user())) {
        return 0;
    }
    return user->query_setting(setting);
}