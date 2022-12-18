void command (string input, mapping flags) {
    string s = " ", msg = "";
    int width = this_user()->query_account()->query_setting("width");
    int w = width * 2;
    mixed *dots;
    int r;

    if (flags["r"]) {
        r = max(({ 2, to_int(flags["r"]) }));
    } else {
        r = 6;
    }

    dots = allocate(w/2, (: allocate($(w/4), (:
        0 |
        (!random($($(r))) ? 0x1 : 0) |
        (!random($($(r))) ? 0x2 : 0) |
        (!random($($(r))) ? 0x4 : 0) |
        (!random($($(r))) ? 0x40 : 0) |
        (!random($($(r))) ? 0x8 : 0) |
        (!random($($(r))) ? 0x10 : 0) |
        (!random($($(r))) ? 0x20 : 0) |
        (!random($($(r))) ? 0x80 : 0)
    :)) :));

    for (int y = 0; y < w/4; y ++) {
        for (int x = 0; x < w/2; x ++) {
            s[0] = 10240 + dots[x][y];
            msg += s;
        }
        msg += "\n";
    }
    message("system", msg, this_user());
}
