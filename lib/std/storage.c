inherit STD_ITEM;

inherit M_CONTAINER;

string query_long () {
    string long = item::query_long();
    mixed *list;
    string *shorts, conjunctions;

    list = unique_array(query_item_contents(), (: $1->query_short() :));
    if (sizeof(list)) {
        shorts = sort_array(map_array(list, (: consolidate(sizeof($1), $1[0]->query_short()) :)), 1);
        shorts[0] = capitalize(shorts[0]);
        conjunctions = conjunction(shorts);
        return long + "\n\n" + conjunctions + " " + (regexp(conjunctions, " and ") ? "are" : "is") + " inside.";
    } else {
        return long;
    }
}

/* ----- parser applies ----- */

mixed indirect_get_obj_from_obj (mixed args...) {
    object env = environment();
    if (env != environment(previous_object())) {
        if (env == previous_object()) {
            return 0;
        } else {
            return "You can't get what isn't here.";
        }
    }
    return 1;
}
mixed indirect_put_obj_in_obj (mixed args...) {
    if (!args[0]) return 0;
    if (environment(args[0]) != previous_object()) {
        return "You can't put what you don't have.";
    }
    return 1;
}