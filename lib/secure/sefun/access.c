mixed unguarded (closure fn) {
    debug_message("sefun/acess->unguarded\n");
    if (!fn) raise_error("Bad argument 1 to access->unguarded");
    return D_ACCESS->unguarded(fn);
}