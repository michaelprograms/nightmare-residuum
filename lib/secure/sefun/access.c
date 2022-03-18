mixed unguarded (function fn) {
    if (undefinedp(fn)) error("Bad argument 1 to access->unguarded");
    return D_ACCESS->unguarded(fn);
}