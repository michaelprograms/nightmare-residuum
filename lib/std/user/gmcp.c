// @this_object /std/user.c
nosave mapping __GMCPModuleData = ([]);

int gmcp_enabled() {
    if (this_object()->query_setting("gmcp") != "on") {
        return 0;
    }
    return has_gmcp(this_object());
}

void gmcp_send_update(string module, mapping data) {
    if (!gmcp_enabled()) {
        return;
    }
    if (!mapp(__GMCPModuleData)) {  // coverage: ignore
        __GMCPModuleData = ([]);  // coverage: ignore
    }  // coverage: ignore
    if (!mapp(__GMCPModuleData[module])) {  // coverage: ignore
        __GMCPModuleData[module] = ([]);  // coverage: ignore
    }  // coverage: ignore
    __GMCPModuleData[module] += data;  // coverage: ignore

    send_gmcp(module + " " + json_encode(data));  // coverage: ignore
}

void gmcp(string request) {
    if (request == "Char.Vitals.Get") {
        object char = this_object()->query_character();
        if (!char) {
            return;
        }

        char->gmcp_update_vitals();
    } else if (request == "Room.Info.Get") {
        object char = this_object()->query_character();
        /** @type {STD_ROOM} env */
        object env;

        if (!char) {
            return;
        }
        env = environment(char);
        gmcp_send_update("Room.Info", env ? ([
            "name": env->query_short(),
            "exits": env->query_exit_directions(),
            "area": explode(base_name(env), "/")[1],
        ]) : ([
            "name": "",
            "exits": "",
            "area": "",
        ]));
    }

}
