string sanitize_name (string name);

object this_account () {
    object a;
    if (this_player() && (a = this_player()->query_account())) return a;
    else return 0;
}
object this_character () {
    object c;
    foreach (object ob in ({ previous_object(), previous_object(1), this_player() })) {
        if (ob && (c = ob->query_character())) {
            break;
        }
    }
    return c;
}
object find_character (string name) {
    object *results = filter(clones(STD_CHARACTER), (: $1 && $1->query_key_name() == $2:), sanitize_name(name));
    return sizeof(results) ? results[0] : 0;
}

object *characters () {
    return map(filter(users() || ({}), (: $1 && interactive($1) && $1->query_character() :)) || ({}), (: $1->query_character() :)) || ({});
}