private mapping __Properties = ([]);

mapping query_properties ();
mixed query_property (string key);
mixed set_property (string key, mixed value);
mapping set_properties (mapping properties);
mixed add_property (string key, mixed value);
int remove_property (string key);

mapping query_properties () {
    if (!mappingp(__Properties)) __Properties = ([]);
    return __Properties;
}
mixed query_property (string key) {
    if (!stringp(key) || key == "") raise_error("Bad argument 1 to property->query_property");
    if (!mappingp(__Properties)) __Properties = ([]);
    return __Properties[key];
}
mapping set_properties (mapping properties) {
    if (!mappingp(properties) || !sizeof(properties)) raise_error("Bad argument 1 to property->set_properties");
    if (!mappingp(__Properties)) __Properties = ([]);
    if (sizeof(__Properties)) __Properties += properties;
    else __Properties = properties;
    return __Properties;
}
mixed set_property (string key, mixed value) {
    if (!stringp(key) || key == "") raise_error("Bad argument 1 to property->set_property");
    if (!value || value == "") raise_error("Bad argument 2 to property->set_property");
    if (pointerp(value) || mappingp(value)) raise_error("Bad argument 2 to property->set_property");
    if (!mappingp(__Properties)) __Properties = ([]);
    return __Properties[key] = value;
}
mixed add_property (string key, mixed value) {
    if (!stringp(key) || key == "") raise_error("Bad argument 1 to property->add_property");
    if (!value || value == "") raise_error("Bad argument 2 to property->add_property");
    if (pointerp(value) || mappingp(value)) raise_error("Bad argument 2 to property->add_property");
    if (!mappingp(__Properties)) __Properties = ([]);
    if (__Properties[key]) __Properties[key] += value;
    else __Properties[key] = value;
    return __Properties[key];
}
int remove_property (string key) {
    if (!stringp(key) || key == "") raise_error("Bad argument 1 to property->remove_property");
    if (!mappingp(__Properties)) __Properties = ([]);
    if (!!__Properties[key]) map_delete(__Properties, key);
    return !__Properties[key];
}