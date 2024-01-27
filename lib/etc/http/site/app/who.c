mapping data (mapping response, string *args) {
    mapping data = ([ ]);

    foreach (object character in characters()) {
        data[character->query_name()] = ([
            "level": character->query_level(),
            "species": character->query_species(),
        ]);
    }

    return data;
}