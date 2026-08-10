// HTTP handler: GET /app/planet/<name> renders an existing planet to a PNG and
// returns the image bytes. Reuses D_PLANET->generate_png (the native FFI
// renderer) which writes /tmp/<name>-<size>.png, then streams the file back.
// The planet must already exist (created via D_PLANET->create_planet).

// read_buffer is capped by the driver's "maximum byte transfer" config, so a
// large PNG can't be read in one call. Read it in sub-cap chunks and join them.
private buffer read_whole(string path) {
    buffer data, part;
    int off, chunk;

    chunk = 200000;  // under "maximum byte transfer" (256000)
    off = 0;
    data = 0;
    while (bufferp(part = read_buffer(path, off, chunk)) && sizeof(part)) {
        data = data ? data + part : part;
        off += sizeof(part);
        if (sizeof(part) < chunk) {
            break;
        }
    }
    return data;
}

mixed png(mapping response, string *args) {
    string name, path;
    mapping planet;
    buffer data;

    if (!pointerp(args) || !sizeof(args) || !stringp(args[0]) || !sizeof(
        args[0]
    )) {
        return ([ "error": "missing planet name" ]);
    }
    name = args[0];
    // native renderer writes the path directly; keep it free of traversal
    if (strsrch(name, "/") >= 0 || strsrch(name, "..") >= 0) {
        return ([ "error": "invalid planet name" ]);
    }
    planet = D_PLANET->query_planet(name);
    if (!mapp(planet) || !planet["size"]) {
        return ([ "error": "unknown planet: " + name ]);
    }
    if (!D_PLANET->generate_png(name)) {
        return ([ "error": "render failed for: " + name ]);
    }
    path = "/tmp/" + name + "-" + planet["size"] + ".png";
    data = read_whole(path);
    if (!bufferp(data)) {
        return ([ "error": "could not read rendered image" ]);
    }
    response["type"] = "image/png";
    return data;
}
