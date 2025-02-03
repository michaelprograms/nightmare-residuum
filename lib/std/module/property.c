mapping __Properties = ([ ]);

private void initialize_properties () {
    if (!mapp(__Properties)) {
        __Properties = ([ ]);
    }
}

mapping query_properties () {
    initialize_properties();
    return __Properties;
}
mixed query_property (string key) {
    if (!stringp(key) || key == "") {
        error("Bad argument 1 to property->query_property");
    }
    initialize_properties();
    return __Properties[key];
}

mixed set_property (string key, mixed value) {
    if (!stringp(key) || key == "") {
        error("Bad argument 1 to property->set_property");
    }
    if (undefinedp(value)) {
        error("Bad argument 2 to property->set_property");
    }
    if (arrayp(value) || mapp(value)) {
        error("Bad argument 2 to property->set_property");
    }
    initialize_properties();
    return __Properties[key] = value;
}
mixed add_property (string key, mixed value) {
    if (!stringp(key) || key == "") {
        error("Bad argument 1 to property->add_property");
    }
    if (undefinedp(value) || value == "") {
        error("Bad argument 2 to property->add_property");
    }
    if (arrayp(value) || mapp(value)) {
        error("Bad argument 2 to property->add_property");
    }
    initialize_properties();
    if (__Properties[key]) {
        __Properties[key] += value;
    } else {
        __Properties[key] = value;
    }
    return __Properties[key];
}
mapping set_properties (mapping properties) {
    if (!mapp(properties) || !sizeof(properties)) {
        error("Bad argument 1 to property->set_properties");
    }
    initialize_properties();
    __Properties += copy(properties);
    return __Properties;
}

int remove_property (string key) {
    if (!stringp(key) || key == "") {
        error("Bad argument 1 to property->remove_property");
    }
    initialize_properties();
    if (!undefinedp(__Properties[key])) {
        map_delete(__Properties, key);
    }
    return !__Properties[key];
}
mapping remove_properties () {
    return __Properties = ([ ]);
}