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