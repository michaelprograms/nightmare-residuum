nosave private mapping __Properties = ([]);

mapping query_properties () {
    return __Properties;
}
mixed query_property (string key) {
    if (!stringp(key) || key == "") error("Bad argument 1 to property->query_property");
    return __Properties[key];
}
mapping set_properties (mapping properties) {
    if (!mapp(properties) || !sizeof(properties)) error("Bad argument 1 to property->set_properties");
    if (sizeof(__Properties)) __Properties += properties;
    else __Properties = properties;
    return __Properties;
}
mixed set_property (string key, mixed value) {
    if (!stringp(key) || key == "") error("Bad argument 1 to property->set_property");
    if (undefinedp(value) || value == "") error("Bad argument 2 to property->set_property");
    if (arrayp(value) || mapp(value)) error("Bad argument 2 to property->set_property");
    return __Properties[key] = value;
}
mixed add_property (string key, mixed value) {
    if (!stringp(key) || key == "") error("Bad argument 1 to property->add_property");
    if (undefinedp(value) || value == "") error("Bad argument 2 to property->add_property");
    if (arrayp(value) || mapp(value)) error("Bad argument 2 to property->add_property");
    if (__Properties[key]) __Properties[key] += value;
    else __Properties[key] = value;
    return __Properties[key];
}
int remove_property (string key) {
    if (!stringp(key) || key == "") error("Bad argument 1 to property->remove_property");
    if (!undefinedp(__Properties[key])) map_delete(__Properties, key);
    return !__Properties[key];
}