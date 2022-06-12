mixed unguarded (function fn) {
    if (!functionp(fn)) error("Bad argument 1 to access->unguarded");
    return D_ACCESS->unguarded(fn);
}